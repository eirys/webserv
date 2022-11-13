<html>
	<head>
		<title>Cutie webserv 😚 💖</title>
		<meta charset="UTF-8">
	</head>
    <style type="text/css">
    body {
    margin: 0;
    padding: 0;
    }

    @font-face {
    font-family: berry;
    src: url(../fonts/Berry_Merry.otf);
    }

    @font-face {
    font-family: peralta;
    src: url(../fonts/Peralta-Regular.ttf);
    }

    @font-face {
	font-family: NotoSerifKR_R;
	src: url(../fonts/NotoSerifKR-Bold.otf)
    }

    @font-face {
	font-family: NotoSerifKR_L;
	src: url(../fonts/NotoSerifKR-Light.otf)
    }

    h1 {
    color: #d71b3b;
    font-family: peralta;
    }

	h2 {
    color: #0000FF;
    font-family: peralta;
    }

	.background {
	background: url(../html/img/hamster_cookie_2.jpg) no-repeat center center fixed;
	-webkit-background-size: cover;
	-moz-background-size: cover;
	-o-background-size: cover;
	background-size: cover;
	}

    .bouton {
	margin: 20px;
	background-color: #FF69B4;
	color: white;
	text-align: center;
	font-family: berry;
	font-size: 150%;
	box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);
    }
    </style>
	<body class="background">
		<h1>
			<center>Test Cookie</center>
		</h1>
		<h2>
			<center>Session ID</center>
		</h2>
		<?php
			...
		?>
		</div>
			<form action="../form.html">
				<center><input type="submit" value="Go acheter un nouvel ami !" class="bouton" /></center>
			</form>
			<form action="cgi_achat.php">
				<center><input type="submit" value="Go voir le panier achete !" class="bouton" /></center>
			</form>
            <form action="cgi_destroy.php">
				<center><input type="submit" value="Jeter votre hamster et reformuler une adoption" class="bouton" /></center>
			</form>
			<form action="../index.html">
				<center><input type="submit" value="Retour vers la page d'accueil" class="bouton" /></center>
			</form>
		</div>
	</body>
</html>
