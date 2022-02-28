#sudo apt update

#Install Packages
#sudo apt install -y gcc binutils build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev g++

export PREFIX="/usr/local"
#

export TARGET=x86_64-elf

#TARGET EXAMPLE
#x86_64: x86_64-elf
#i386: i386-elf
#i686: i686-elf
#aarch: aarch64-elf
#powerpc: powerpc-elf

export THREADOFNUMBER=8

export PATH=$PATH:$PREFIX/bin

#WGET GCC AND BINUTILS
#wget https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz
#wget https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.gz

#tar -xvf binutils-2.37.tar.gz
#tar -xvf gcc-11.2.0.tar.gz

#sudo mv binutils-2.37 $PREFIX
#sudo mv gcc-11.2.0 $PREFIX

cd $PREFIX
sudo mkdir $TARGET-build-gcc
sudo mkdir $TARGET-build-binutils

#BUILDING-BINUTILS
cd $PREFIX/$TARGET-build-binutils
sudo ../binutils-2.37/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
sudo make -j $THREADNUMBERS
sudo make install -j $THREADNUMBERS

#BUILDING-GCC
cd $PREFIX/$TARGET-build-gcc
sudo ../gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
sudo make all-gcc -j $THREADNUMBERS
sudo make all-target-libgcc -j $THREADNUMBERS
sudo make install-gcc -j $THREADNUMBERS
sudo make install-target-libgcc -j $THREADNUMBERS

#VERSION
$TARGET-gcc --version
