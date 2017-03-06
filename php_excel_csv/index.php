<?php
/**
 * As a demo
 */
?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>PHP EXCEL CSV - XLSX/IO Sinri Version</title>
	<style type="text/css">
		iframe {
			width: 80%;
			height: 300px;
		}
	</style>
</head>
<body>
	<h1>PHP EXCEL CSV - XLSX/IO Sinri Version</h1>
	<h2>Upload and analyze demo</h2>
	<hr>
	<form action="./php_excel_csv.php" enctype="multipart/form-data" method="POST" target="result_iframe">
		Excel File (*.xls, *.xlsx): <input type="file" name="file">
		<input type="hidden" name="MAX_FILE_SIZE" value="10000000" />
		<input type="submit" value="Submit" />
	</form>
	<hr>
	<iframe name="result_iframe"></iframe>
</body>
</html>