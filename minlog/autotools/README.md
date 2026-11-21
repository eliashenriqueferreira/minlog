# minlog in autotools development environment

<br>First of all install the autotools
```bash
sudo dnf groupinstall -y "Development Tools"
or 
sudo dnf install -y autoconf automake libtool make gcc gcc-c++
```

<br>Go to minlog/autotools and make this
```bash
autoreconf -i
mkdir build
cd build
../configure
make clean
make
```

<br>----------------------------------------------------------------------------



