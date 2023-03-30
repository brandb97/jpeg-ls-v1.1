cd ../Encoder
make jpeglse
cd ../Decoder
make jpeglsd
cd ../Test

touch compressed uncompressed

../Encoder/jpeglse test_image compressed 4 4 32
../Decoder/jpeglsd compressed uncompressed

echo -e "test_image:\n"
cat test_image
echo -e "\n"
echo -e "after compress and uncompress\n"
cat uncompressed
echo -e "\n"

rm compressed uncompressed

cd ../Encoder
make clean
cd ../Decoder
make clean
cd ../Test
