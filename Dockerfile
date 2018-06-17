FROM greyltc/archlinux
RUN pacman -Syyu --noconfirm
RUN pacman -S core/gcc --noconfirm
RUN pacman -S nasm --noconfirm
RUN pacman -S core/grub --noconfirm
RUN pacman -S xorriso --noconfirm
RUN pacman -S mtools --noconfirm
#ADD source /root/helvede/source
#ADD output /root/helvede/output
ADD build.sh gcc_options /root/helvede/
