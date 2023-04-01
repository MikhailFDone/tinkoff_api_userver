cd ./build/proto/
sed -i 's/namespace public/namespace public_/g; s/::public/::public_/g' *.pb.*  