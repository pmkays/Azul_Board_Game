for i in {1..7}
do
    if((${i} == 5 || ${i} == 6 || ${i} == 7))
    then
        ./assign2 -s 2 <${i}commands.in >consoleOutput.txt
        diff ${i}expectedoutcome.save actualoutcome.save
        if [ $? -ne 0 ]; 
        then
            echo "-->Failed test ${i}!"
        else
            echo "Passed test ${i}!"
        fi
    else
        ./assign2 -s 2 <${i}commands.in >consoleOutput.txt
        diff ${i}expectedoutcome.save ${i}actualoutcome.save
        if [ $? -ne 0 ];
        then
            echo "-->Failed test ${i}!"
        else
            echo "Passed test ${i}!"
        fi
    fi
done
