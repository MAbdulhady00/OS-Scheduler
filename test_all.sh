./test_generator.out $(((RANDOM % 30)+1))
./process_generator.out 1 > /dev/null
node TestScheduling.js > out.log
FILE1="out.log"
FILE2="scheduler.log"
DIFF=$(diff "$FILE1" "$FILE2")
if [ "$DIFF" != "" ]; then
  echo "Error in test case"
else
  echo "test case successful!"
fi
