#!/bin/bash
#DEBUG=NO
DEBUG=YES
MERGFILE="./mergfile"
SORTMERG="./sortmerg"
RESTFILE="./result"
SOURFILE="./data/data*.txt"
linenum=
colum1=
colum2=
last_colum1=
last_colum2=
result=
if [ -f $SORTMERG ];then
   sudo rm $SORTMERG
fi
if [ -f $MERGFILE ];then
   sudo rm $MERGFILE
fi
if [ -f $RESTFILE ];then
   sudo rm $RESTFILE
fi

cat $SOURFILE > $MERGFILE
sort $MERGFILE | uniq -c > $SORTMERG

if [ $DEBUG == YES ];then       #如果开启调试参数
echo "-----mersort-----"
cat $SORTMERG
echo "-----result------"
fi

cat $SORTMERG | while read line
do
    let 'linenum++'
    colum1=`echo $line | awk '{print $1}'`
    colum2=`echo $line | awk '{print $2}'`

    #如果下一行与上一行相同，进行相加或相减操作
    if [[ $linenum -gt "1" && ${colum2:1:5} == ${last_colum2:1:5} ]];then
       ch=${colum2:0:1}
       last_ch=${last_colum2:0:1}
       if [[ $ch == "+" && $last_ch == "-" ]];then
          result=`expr $colum1 - $last_colum1`
       elif [ $ch == "-" && $last_ch == "+" ];then
          result=`expr $last_colum1 - $colum1`
       fi

       sed -i '$d' $RESTFILE      
       #如果计算值为0，则不进行统计
       if [ $result != "0" ];then
          echo "$result " " ${colum2:1:5}" >> $RESTFILE
       fi
       continue
    fi

    last_colum1=$colum1
    last_colum2=$colum2
    if [ ${colum2:0:1} == "-" ];then
       colum1=`expr 0 - $colum1`
    fi
    echo "$colum1 " " ${colum2:1:5}" >> $RESTFILE
done

if [ $DEBUG == YES ];then
cat $RESTFILE
fi

##############对结果文件进行统计################
scene0=0
scene1=0
scene2=0
scene3=0
scene4=0
while read line
do
    colum1=`echo $line | awk '{print $1}'`
    colum2=`echo $line | awk '{print $2}'`
    if [ ${colum2:0:1} == "0" ];then
         scene0=`expr $colum1 + $scene0`
    elif [ ${colum2:0:1} == "1" ];then
         scene1=`expr $colum1 + $scene1`
    elif [ ${colum2:0:1} == "2" ];then
         scene2=`expr $colum1 + $scene2`
    elif [ ${colum2:0:1} == "3" ];then
         scene3=`expr $colum1 + $scene3`
    elif [ ${colum2:0:1} == "4" ];then
         scene4=`expr $colum1 + $scene4`
    fi

    totalnum=`expr $scene0 + $scene1 + $scene2 + $scene3 + $scene4`
done <$RESTFILE
echo "----calculate----"
echo "|scene0|:$scene0"
echo "|scene1|:$scene1"
echo "|scene2|:$scene2"
echo "|scene3|:$scene3"
echo "|scene4|:$scene4"
echo " totalnum: $totalnum"
rm $MERGFILE
rm $SORTMERG
rm $RESTFILE

