awk '{total+=-bash}END{print total}' lsum > lsumf && truncate -s 10 lsumf
