#!/bin/bash

factorial(){      # функция считающая фактриал
	i=2            # счётчик
	res=1            # результат вычислений
	while [ $i -le $1 ] # цикл от 2 до числа для которого считается факториал
	do
		res=$((res*i))
		i=$((i+1))    # подсчёт факториала и увеличение счётчика
	done
	echo $res   # возвращаем результат через echo тк return принимает только значения от 0 до 255
}

echo "Enter number to count factorial"
read num  
if [ $num -ge 1 ]; then        # если число для факториала больше или равно 1 считаем факториал 
result=$(factorial $num)
echo "result $result"

else                      # иначе сообщаем что число не подходит
echo "bad number"
fi
