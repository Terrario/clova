#!/bin/bash

SENTENCES_SQL="select 'https://audio.tatoeba.org/sentences/rus/' || t.sentence_id || '.mp3' from translation t inner join sentence_audio sa on sa.sentence_id = t.sentence_id where sa.license is not null;"
IDS=`echo $SENTENCES_SQL | sqlite3 db.sqlite3`

for audio in $IDS
do
    out=`echo $audio | cut -d'/' -f6`
    curl $audio -o audio/$out
done

