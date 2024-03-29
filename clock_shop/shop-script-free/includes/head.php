<?php
/*****************************************************************************
 *                                                                           *
 * Shop-Script FREE                                                          *
 * Copyright (c) 2005 WebAsyst LLC. All rights reserved.                     *
 *                                                                           *
 ****************************************************************************/

	// head variables definition: title, meta

	$name = "name";
	if($_SESSION["current_language"] == 1)
		$name = "name_de";
	if($_SESSION["current_language"] == 2)
		$name = "name_en";

	// title
	$r = array(); $r[0] = "";
	if (isset($categoryID) && !isset($productID) && $categoryID>0)
	{
		$q = db_query("SELECT ".$name." FROM ".CATEGORIES_TABLE." WHERE categoryID<>0 and categoryID='$categoryID'") or die (db_error());
		$r = db_fetch_row($q);
		$page_title = str_replace("\"","'",$r[0]." - ".CONF_SHOP_TITLE);
	}
	else if (isset($productID) && $productID>0)
	{
		$q = db_query("SELECT ".$name." FROM ".PRODUCTS_TABLE." WHERE productID='$productID'") or die (db_error());
		$r = db_fetch_row($q);
		$page_title = str_replace("\"","'",$r[0]." - ".CONF_SHOP_TITLE);
	}
	else $page_title = CONF_SHOP_TITLE;


	// META
	$r = array(); $r[0] = "";
	if (isset($categoryID) && !isset($productID) && $categoryID>0)
	{
		$q = db_query("SELECT ".$name.", description FROM ".CATEGORIES_TABLE." WHERE categoryID<>0 and categoryID='$categoryID'") or die (db_error());
		$r = db_fetch_row($q);
		$page_meta = str_replace("\"","'",$r[0].", ".$r[1]);
	}
	else if (isset($productID) && $productID>0)
	{
		$q = db_query("SELECT ".$name.", brief_description FROM ".PRODUCTS_TABLE." WHERE productID='$productID'") or die (db_error());
		$r = db_fetch_row($q);
		$page_meta = str_replace("\"","'",$r[0].", ".$r[1]);
	}
	else $page_meta = CONF_SHOP_META;



	$smarty->assign("page_meta",$page_meta);
	$smarty->assign("page_title",$page_title); 
?>
