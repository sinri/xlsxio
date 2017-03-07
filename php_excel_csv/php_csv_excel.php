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
    /*
    $charset_result=exec("enca ".escapeshellarg($_FILES["file"]["tmp_name"]));
    if(FALSE===strstr($charset_result, "UTF-8")){
        // exec("enca -x utf-8 ".escapeshellarg($_FILES["file"]["tmp_name"]));
        exec("iconv -t UTF-8 -f GBK -c ".escapeshellarg($_FILES["file"]["tmp_name"])."  > ".escapeshellarg($_FILES["file"]["tmp_name"]),$output_lines,$return_var);
        if($return_var){
            //error
            $cc=file_get_contents($_FILES["file"]["tmp_name"]);
            response("FAIL","Convert Output: ".$cc);
        }
    }
    */

    exec("SinriCSV2Excel ".escapeshellarg($_FILES["file"]["tmp_name"])." ".escapeshellarg($_FILES["file"]["tmp_name"].".xlsx"),$output_lines,$return_var);
    if($return_var){
        //error
        response("FAIL","Convert Error: ".implode("\n", $output_lines));
    }else{
        $xlsx_filename=$_FILES["file"]["tmp_name"].".xlsx";

        header('Content-Description: File Transfer'); //描述页面返回的结果
        header('Content-Type: application/octet-stream'); //返回内容的类型，此处只知道是二进制流。具体返回类型可参考http://tool.oschina.net/commons
        header('Content-Disposition: attachment; filename='.basename($xlsx_filename));//可以让浏览器弹出下载窗口
        header('Content-Transfer-Encoding: binary');//内容编码方式，直接二进制，不要gzip压缩
        header('Expires: 0');//过期时间
        header('Cache-Control: must-revalidate');//缓存策略，强制页面不缓存，作用与no-cache相同，但更严格，强制意味更明显
        header('Pragma: public');
        header('Content-Length: ' . filesize($xlsx_filename));//文件大小，在文件超过2G的时候，filesize()返回的结果可能不正确

        $file = @fopen($xlsx_filename,"rb");
        while(!feof($file))
        {
            print(@fread($file, 1024*8));
            ob_flush();
            flush();
        }
        exit();
    }
}

function response($result,$data){
    echo json_encode(array("result"=>$result,"data"=>$data),JSON_PRETTY_PRINT);
    exit();
}