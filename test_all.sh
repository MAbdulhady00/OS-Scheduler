./test_generator.out $(((RANDOM % 30)+1))
./process_generator.out 1 > /dev/null
node TestScheduling.js 1 > out.log
FILE1="out.log"
FILE2="scheduler.log"
DIFF=$(diff "$FILE1" "$FILE2")
if [ "$DIFF" != "" ]; then
  echo "Error in HPF test case:"
  echo $DIFF
else
  echo "HPF test successful!"
fi
./process_generator.out 2 > /dev/null
node TestScheduling.js 2 > out.log
python3 testValidation.py "SRTN test successful!"
quantum=$(((RANDOM % 8)+1))
echo testing with quantum: $quantum
./process_generator.out 3 $quantum > /dev/null
node TestScheduling.js 3 $quantum > out.log
python3 testValidation.py "RR test successful!"
