#!/bin/bash

check_number(){          # функция проверки равенства чисел
if [ $2 -eq $1 ]; then    # еси числа равны выводит да иначе нет
	echo "Yes"
else
	echo "No"
fi;
}

a=10
b=0
echo "Guess my number"      

while [ $b -ne $a ]        # пока числе не равны проверяем их равенство
do
	read b                  # считываем число
	check_number $a $b      # проверяем число
done
