<?php
	include 'defines.php';

	OpenPage("Main");
	MainMenu("Main");

	session_start();
	

	$lnk = mysql_connect($DbServerAddr, $DbServerName, $DbServerPwd) or die ($DbErrorConnect . mysql_error());
	mysql_select_db($DbName, $lnk) or die ($DbErrorSelect . mysql_error());


	// WELCOME
	if($_SESSION['authorized'] == true)
	{
		printf("<div class='header'> Добро пожаловать, %s!</div>\n", $_SESSION['UserName']);		
	}
	else
	{
		printf("<div class='header'>Добро пожаловать!</div>\n");

		$_SESSION["returnToPage"] = "Main";
		printf("<div class='wrapper'> <a href=auth.php>Вход</a><br> </div>\n");
        printf("<div class='wrapper'> <a href=reg.php>Регистрация</a> </div>\n");
	}


	// TOP 10 TABLE 
	printf("<div class='wrapper'>Десятка лучших игроков:</div>\n");
	$result = mysql_query("SELECT wsUsers.User, tbChessRating.Rating FROM wsUsers 
                           INNER JOIN tbChessRating ON wsUsers.GUID = tbChessRating.PlayerID 
						   WHERE tbChessRating.Available = 1
                           ORDER by tbChessRating.Rating DESC
						   LIMIT 10") or die("Invalid query: " . mysql_error());

	printf("<table class='rating_table'>\n");
    printf("<td> Место </td>  <td> Игрок </td> <td> Рейтинг </td>\n");  

	$i = 1;
	while ($row = mysql_fetch_array($result, MYSQL_NUM)) 
    {
		if($_SESSION['authorized'] == true && $row[0] == $_SESSION['UserName'])
			 $class = "table_red";
		else
			 $class = "table_black";

		printf ("<tr> <td class=%s> %s </td> <td  class=%s> %s </td> <td  class=%s> %s </td> </tr>\n", $class,  $i, $class, $row[0], $class, $row[1]);  
		$i++;
	}
    printf("</table>\n");


//	$num_rows = mysql_num_rows($result);
//	echo "$num_rows Rows\n";
	
	ClosePage();
?> 

