pkgname=nrCascadeSim
pkgver=1.5.0
pkgrel=1
pkgdesc="Library for computing neutron capture-induced nuclear recoil spectra."
arch=('x86_64')
license=(MIT)
depends=('root>=6')
makedepends=('gcc>=4.4.7' 'cern-vdt')
url="https://github.com/villano-lab/nrCascadeSim"

source=("${url}/archive/refs/tags/v${pkgver}.tar.gz")
md5sums=('c8ff62ecbb3ef2b9800f40896d3baced')

srcdir=src/${pkgname}-${pkgver}
builddir=build
installdir=install

build(){
	mkdir -p $builddir
	cd $builddir
	echo cmake -DCMAKE_INSTALL_PREFIX="${installdir}" ${srcdir}
	cmake -DCMAKE_INSTALL_PREFIX="${installdir}" ${srcdir}/${pkgname}-${pkgver}
	make
}

package() {
	cd $builddir
	make install
}
