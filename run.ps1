docker run --rm -v "$(pwd):/build" ghcr.io/ariahiro64/altra64:master make clean
docker run --rm -v "$(pwd):/build" ghcr.io/ariahiro64/altra64:master make
mv "out/fruit64.z64" "out/OS64P.v64"
B:\programs\cen64\cen64.bat "out/OS64P.v64"