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

srcdir=${pkgdir}/src/${pkgname}-${pkgver}

build(){
	echo $srcdir
	mkdir -p ${srcdir}/build
	cd ${srcdir}/build
	cmake -DCMAKE_INSTALL_PREFIX="${pkgdir}" ${srcdir}/${pkgname}-${pkgver}
	make
}

package() {
	cd ${srcdir}/build
	make install
}
