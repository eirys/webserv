<?php
	session_start();
?>
<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
		<link href="cgi_destroy.css" rel="stylesheet">
		<link rel="icon" href="../img/favicon.ico">
	</head>
	<body class="background">
		<div class="icon">
			<a href="../form_order"><img src="../img/icon_panier_no_cgi.png" width="100px"></a>
		</div>
		<div class="icon_cgi">
			<a href="cgi_achat.php"><img src="../img/icon_panier_with_cgi.png" width="100px"></a>
		</div>
		<div class="paint">
			<a href="../form_gallery"><img src="../img/icon_paint.png" width="100px"></a>
		</div>
		<div class="house">
			<a href="../index.html"><img src="../img/icon_house.png" width="100px"></a>
		</div>
		<div class="danse">
			<a href="http://gcasale.free.fr/viva.htm"><img src="../img/icon_dance.gif" width="90px"></a>
		</div>
		<h1>
			Suppression de votre panier 💔
		</h1>
		<div class="hamster">
			<img src="../img/hamster_trash.png" width="600px">
		</div>
		<div class="phrase">
        <?php
	    	if (isset($_SESSION['cart']))
            {
				session_destroy();
				echo "En esperant que vous avez apprecié cette experience d'achat! A bientôt!";
	    	}
            else
            {
	    		echo 'Vous êtes inconnu parmi nos acheteurs pour supprimer votre panier!';
	    	}
        ?>
		</div>
			<form action="../form.html">
				<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
			</form>
			<form action="./cgi_achat.php">
				<center><input type="submit" value="Vérifier si le panier a bien été détruit" class="bouton" /></center>
			</form>
			<form action="../index.html">
				<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
			</form>
		</div>
	</body>
</html>
