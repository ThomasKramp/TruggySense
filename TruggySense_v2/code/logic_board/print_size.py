Import("env")
import subprocess
import os

def print_size(source, target, env):
    elf = str(target[0])
    size_tool = os.path.join(
        env.subst("$PROJECT_PACKAGES_DIR"),
        "toolchain-gccarmnoneeabi-teensy", "bin", "arm-none-eabi-size"
    )
    result = subprocess.run([size_tool, elf], capture_output=True, text=True)
    lines = result.stdout.strip().split("\n")
    if len(lines) >= 2:
        parts = lines[1].split()
        text, data, bss = int(parts[0]), int(parts[1]), int(parts[2])
        flash_used = text + data
        ram_used = data + bss
        flash_total = 7 * 1024 * 1024 + 750 * 1024  # 7.75MB
        ram_total = 512 * 1024

        def bar(used, total, width=10):
            filled = int(round(used / total * width))
            return "[" + "=" * filled + " " * (width - filled) + "]"

        print(f"RAM:   {bar(ram_used, ram_total)}  {ram_used/ram_total*100:.1f}% (used {ram_used} bytes from {ram_total} bytes)")
        print(f"Flash: {bar(flash_used, flash_total)}  {flash_used/flash_total*100:.1f}% (used {flash_used} bytes from {flash_total} bytes)")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", print_size)