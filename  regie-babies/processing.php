<?php
 $query=$_POST['query'];

exec('./oursql "'.$query.'"',$output);

print_r($output);
?>
