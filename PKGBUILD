pkgname=pretty-c
pkgver=1.0.0
pkgrel=1
pkgdesc="Make C look like many other languages while retaining its charecteristics. not how god intended."
arch=("any")
url="https://github.com/aartaka/pretty.c"
license=("WTFPL")
source=("pretty.h")
md5sums=("SKIP")
optdepends=(
	'clang: if you want to use clang'
	'gcc: if you want to use gcc'
)

package() {
	install -Dm644 pretty.h "$pkgdir/usr/include/pretty.h"
}
