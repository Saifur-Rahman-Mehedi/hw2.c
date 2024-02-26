#!/bin/bash
sed ':a;N;$!ba;s/\n/ /g' "/tmp/exp.txt" | tr -s ' ' > "/tmp/exp_single_line.txt"
sed ':a;N;$!ba;s/\n/ /g' "/tmp/act.txt" | tr -s ' ' > "/tmp/act_single_line.txt"
