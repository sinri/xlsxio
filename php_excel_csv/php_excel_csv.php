<?php
/**
 * POST TO THIS FILE
 * RESPONSE CSV
 * PHP 5.4 or later required
 */
//允许跨域……跨域……只有POST可以
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods:POST');

if(!isset($_FILES["file"])){
    response("FAIL","File Upload Error: Not Yet");
}

if ($_FILES["file"]["error"] > 0){
    // echo "Error: " . $_FILES["file"]["error"] . "<br />";
    response("FAIL","File Upload Error: ".$_FILES["file"]["error"]);
}
else{
    // echo "Upload: " . $_FILES["file"]["name"] . "<br />";
    // echo "Type: " . $_FILES["file"]["type"] . "<br />";
    // echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
    // echo "Stored in: " . $_FILES["file"]["tmp_name"];

    //FILE SIZE > 10, exit
    if($_FILES["file"]["size"]/1024.0/1024.0>10){
        response("FAIL","File Upload Too Large: ".($_FILES["file"]["size"])." bytes");
    }

    $allowed = array(
        "xls" => array( "application/vnd.ms-excel" ),
        "xlsx" => array(
            "application/vnd.ms-excel",
            "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
            // "application/octet-stream",//For XML written xls? -> not support
        )
    );
    if(!in_array($_FILES['file']['type'], $allowed['xlsx'])){
        response("FAIL","File Upload cannot be treated as EXCEL file: ".$_FILES['file']['type']);
    }
    $command="SinriExcel2CSV";
    if(!empty($_REQUEST['skip_style']) && preg_match('/^\-[a-z]$/', $_REQUEST['skip_style'])){
        $command.=" ".$_REQUEST['skip_style'];
    }
    $command.=" ".escapeshellarg($_FILES["file"]["tmp_name"]);
    if(!empty($_REQUEST['sheet_name'])){
        $command.=" ".escapeshellarg($_REQUEST['sheet_name']);
    }
    $begin_time=microtime(true);
    exec($command,$csv_lines,$return_var);
    $end_time=microtime(true);
    if(!empty($csv_lines)){
        $csv_lines=implode("\n", $csv_lines);
    }else{
        $csv_lines="";
    }
    if($return_var){
        response("FAIL","SinriExcel2CSV Error: ".PHP_EOL.$csv_lines);
    }

    response("OK",array("csv"=>$csv_lines,"exec_time"=>($end_time-$begin_time),"command"=>$command));
}

function response($result,$data){
    echo json_encode(array("result"=>$result,"data"=>$data),JSON_PRETTY_PRINT);
    exit();
}