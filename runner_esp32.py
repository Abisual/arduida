import serial
import time
import python_aida64


def get_info_lines():
    data = python_aida64.getData()

    def get(id_, group='sys', label=None):
        group_data = data.get(group, [])
        for item in group_data:
            if label and item['label'].lower() == label.lower():
                return item['value']
            elif item['id'] == id_:
                return item['value']
        return "N/A"

    cpu_temp = get("TCPU", "temp", "CPU")
    cpu_load = get("SCPUUTI")

    gpu_temp = get("TGPU1", "temp", "GPU")
    gpu_load = get("SGPU1UTI")

    used_vram = get("SUSEDVMEM")
    free_vram = get("SFREEVMEM")
    total_vram = str(int(used_vram) + int(free_vram)) if used_vram.isdigit() and free_vram.isdigit() else "N/A"

    used_ram = get("SUSEDMEM")
    free_ram = get("SFREEMEM")
    total_ram = str(int(used_ram) + int(free_ram)) if used_ram.isdigit() and free_ram.isdigit() else "N/A"

    return [
        f"CPU T: {cpu_temp} C L: {cpu_load}%",
        "",
        f"GPU T: {gpu_temp} C L: {gpu_load}%",
        f"VRAM {used_vram}/{total_vram}",
        "",
        f"RAM: {used_ram}/{total_ram}"
    ]


def get_cpu_info():
    data = python_aida64.getData()
    cpu_temp = next(item['value'] for item in data.get('temp', [])
                    if item.get('label', '').lower() == 'cpu')
    cpu_load = next(item['value'] for item in data.get('sys', [])
                    if item.get('id') == 'SCPUUTI')
    return f"CPU T: {cpu_temp} C L: {cpu_load}%"


def send_to_esp32(port="COM4", baudrate=115200):
    try:
        with serial.Serial(port, baudrate, timeout=2) as ser:
            time.sleep(2)
            ser.reset_input_buffer()

            while True:
                lines = get_info_lines()
                message = "\n".join(lines) + "\nEND\n"
                ser.write(message.encode('utf-8'))
                print(f">>>\n{message.strip()}\n<<<")

                # Ждём подтверждения от ESP32
                response = ser.readline().decode('utf-8', errors='ignore').strip()
                if response.startswith("OK"):
                    print(f"< {response}")

                time.sleep(1)

    except KeyboardInterrupt:
        print("\nОстановлено пользователем")
    except Exception as e:
        print(f"Ошибка: {e}")


if __name__ == "__main__":
    send_to_esp32()


if __name__ == "__main__":
    while True:
        send_to_esp32(get_info_lines())
        time.sleep(1)  # Интервал между отправками