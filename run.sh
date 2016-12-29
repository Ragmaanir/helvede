./build_docker.sh
qemu-system-x86_64 -hda output/image.iso -serial file:stdio -k en-us
