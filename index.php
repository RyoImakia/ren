<?php
        if (preg_match('/on/',$_POST['trigger_word'])):
                exec('./Irsend_kuno_on.c');
                echo '{"text": "エアコンをつけました"}';
        elseif (preg_match('/off/',$_POST['trigger_word'])):
                exec('./Irsend__kuno_off.c');
                echo '{"text": "エアコンをけしました"}';
        endif;
?>