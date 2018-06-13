./build_docker.sh
qemu-system-x86_64 -hda output/image.iso -serial file:stdio -D qemu.log -d int,cpu_reset -no-reboot -no-shutdown

# -gdb tcp::1234
# -singlestep
# -S (start stopped)
