#!/bin/bash

files[1]="test_files/test_1.txt"
files[2]="test_files/test_2.txt"
files[3]="test_files/test_3.txt"
files[4]="test_files/test_4.txt"

test[1]="void"
test[2]="-e ^int -e char"
test[3]="-f test_files/test_template.txt"
test[4]="-f test_files/test_template.txt -e return"
test[5]="-e size -e a -f test_files/test_template.txt"

totsuc=0
totfail=0
vra="./s21_grep"

for (( j=1; j <= 4; j++ ))
do
    for (( i=1; i <= 5; i++ ))
    do
        for k in -i -v -c -l -n -h -s -o
        do
            $vra ${test[$i]} $k ${files[$j]} > s21_grep.txt
            grep ${test[$i]} $k ${files[$j]} > grep.txt
            flag="$(diff -s s21_grep.txt grep.txt)"
            if [ "$flag" == "Files s21_grep.txt and grep.txt are identical" ]
            then
                (( totsuc++ ))
            else
                (( totfail++ ))
            fi
            rm s21_grep.txt grep.txt
            echo "$totsuc $totfail $vra ${test[$i]} $k ${files[$j]}"
        done
        for k in -i -v -c -l -n -h -s -o
        do
            for l in -i -v -c -l -n -h -s -o
            do
                if [ $k != $l ]
                then
                    $vra ${test[$i]} $k $l ${files[$j]} > s21_grep.txt
                    grep ${test[$i]} $k $l ${files[$j]} > grep.txt
                    flag="$(diff -s s21_grep.txt grep.txt)"
                    if [ "$flag" == "Files s21_grep.txt and grep.txt are identical" ]
                    then
                        (( totsuc++ ))
                    else
                        (( totfail++ ))
                    fi
                    rm s21_grep.txt grep.txt
                    echo "$totsuc $totfail $vra ${test[$i]} $k $l ${files[$j]}"
                fi
            done
        done
        for k in -i -v -c -l -n -h -s -o
        do
            for l in -i -v -c -l -n -h -s -o
            do
                for m in -i -v -c -l -n -h -s -o
                do
                    if [ $k != $l ] && [ $m != $l ] && [ $m != $k ]
                    then
                        $vra ${test[$i]} $k $l $m ${files[$j]} > s21_grep.txt
                        grep ${test[$i]} $k $l $m ${files[$j]} > grep.txt
                        flag="$(diff -s s21_grep.txt grep.txt)"
                        if [ "$flag" == "Files s21_grep.txt and grep.txt are identical" ]
                        then
                            (( totsuc++ ))
                        else
                            (( totfail++ ))
                        fi
                        rm s21_grep.txt grep.txt
                        echo "$totsuc $totfail $vra ${test[$i]} $k $l $m ${files[$j]}"
                    fi
                done
            done
        done
        for k in -i -v -c -l -n -h -s -o
        do
            for l in -i -v -c -l -n -h -s -o
            do
                for m in -i -v -c -l -n -h -s -o
                do
                    for n in -i -v -c -l -n -h -s -o
                    do
                        if [ $k != $l ] && [ $m != $l ] && [ $m != $k ] && [ $n != $k ] && [ $n != $l ] && [ $n != $m ]
                        then
                            $vra ${test[$i]} $k $l $m $n ${files[$j]} > s21_grep.txt
                            grep ${test[$i]} $k $l $m $n ${files[$j]} > grep.txt
                            flag="$(diff -s s21_grep.txt grep.txt)"
                            if [ "$flag" == "Files s21_grep.txt and grep.txt are identical" ]
                            then
                                (( totsuc++ ))
                            else
                                (( totfail++ ))
                            fi
                            rm s21_grep.txt grep.txt
                            echo "$totsuc $totfail $vra ${test[$i]} $k $l $m $n ${files[$j]}"
                        fi
                    done
                done
            done
        done
        for k in -i -v -c -l -n -h -s -o
        do
            for l in -i -v -c -l -n -h -s -o
            do
                for m in -i -v -c -l -n -h -s -o
                do
                    for n in -i -v -c -l -n -h -s -o
                    do
                        for z in -i -v -c -l -n -h -s -o
                        do
                            if [ $k != $l ] && [ $m != $l ] \
                            && [ $m != $k ] && [ $n != $k ] \
                            && [ $n != $l ] && [ $n != $m ] \
                            && [ $z != $n ] && [ $z != $m ] \
                            && [ $z != $l ] && [ $z != $k ]
                            then
                                $vra ${test[$i]} $k $l $m $n $z ${files[$j]} > s21_grep.txt
                                grep ${test[$i]} $k $l $m $n $z ${files[$j]} > grep.txt
                                flag="$(diff -s s21_grep.txt grep.txt)"
                                if [ "$flag" == "Files s21_grep.txt and grep.txt are identical" ]
                                then
                                    (( totsuc++ ))
                                else
                                    (( totfail++ ))
                                fi
                                rm s21_grep.txt grep.txt
                                echo "$totsuc $totfail $vra ${test[$i]} $k $l $m $n $z ${files[$j]}"
                            fi
                        done
                    done
                done
            done
        done
    done
done

echo "TOTAL SUCCESS TESTS: $totsuc"
echo "TOTAL FAIL TESTS: $totfail"