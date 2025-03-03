#!/bin/bash
for i in {1..100}; do
	timeout 3s ./philo 5 800 200 200 > output_$i.txt 2>/dev/null
	if grep -q "died" output_$i.txt; then
		echo "Died in run $i at:"
		grep "died" output_$i.txt
	fi
done
echo "Test completed. Checking results..."
grep -l "died" output_*.txt | wc -l | xargs echo "Number of runs with death:"
rm output_*.txt
