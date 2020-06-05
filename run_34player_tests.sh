for i in {1..5}
do
    if((${i} == 3 || ${i} == 4 || ${i} == 5))
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
