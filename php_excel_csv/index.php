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
		<p>
			Excel File (*.xls, *.xlsx): <input type="file" name="file">
			<input type="hidden" name="MAX_FILE_SIZE" value="10000000" />
			&nbsp;&nbsp;
			Sheet Name (Optional, first sheet by default): <input type="text" name="sheet_name">
		</p>
		<p>
			Skip Style: 
			<select name="skip_style">
				<option value=''>Skip empty rows.</option>
				<option value='-c'>Skip empty rows and cells.</option>
				<option value='-i'>Skip rows whose first column is empty.</option>
				<option value='-e'>Skip extra cells right to header row.</option>
				<option value='-n'>Read all cells.</option>
			</select>
			<input type="submit" value="Submit" />
		</p>
	</form>
	<hr>
	<iframe name="result_iframe"></iframe>
	<hr>
	<p style="text-align: center;color:gray;">Copyright 2017 Sinri Edogawa, under <a href="https://github.com/sinri/xlsxio/raw/master/LICENSE.txt">MIT License (MIT)</a>. </p>
</body>
</html>