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

def send_to_arduino(lines, port="COM3", baudrate=9600):
    try:
        with serial.Serial(port, baudrate, timeout=2) as ser:
            time.sleep(2)
            for line in lines:
                ser.write((line + "\n").encode('utf-8'))
                time.sleep(0.05)
            print("✅ Отправлено:")
            for l in lines:
                print(l)
    except Exception as e:
        print(f"❌ Ошибка при отправке: {e}")

if __name__ == "__main__":
    while True:
        send_to_arduino(get_info_lines())
        time.sleep(1)
