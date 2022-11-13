#include "Response.hpp"

std::string		Response::_generateErrorPage()
{
	std::string	errorPage;
	std::string	imagePath;

	imagePath = "/img/hamster_banana.png";
	errorPage = "<!DOCTYPE html>\n\
		<html>\n\
				<head>\n\
					<meta charset=\"utf-8\"/>\n\
					<title>No available page 😰</title>\n\
					<link rel=\"icon\" href=\"../img/favicon.ico\">\n\
				</head>\n\
			<body bgcolor=\"#FFF8BC\">\n\
				<div class=\"hamster\">\n\
					<center>\n\
						<img src=\"https://fantasticfacts.net/wp-content/uploads/2021/12/Drunk-Hamster-1024x768.jpeg\" alt=\"slide\" style=\"width:50%\">\n\
					</center>\n\
				</div>\n\
				<div>\n\
					<center>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							<br><br>Vous êtes ici parce que vous n'avez pas mentionné une page d'erreur spécifique.<br>\n\
							C'est pourquoi vous êtes sur cette page simple, sobre, mais qui reste fonctionnelle.<br><br><br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 250%; font-weight: bold\">\n\
							🥰 Admirez ce hamster heureux. 🥰<br><br><br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							Toutefois, vous pouvez connaître ici le code erreur de cette page qui est :\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%; font-weight: bold\">\n"\
							+ getStatusCodeStr() + "<br>\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%\">\n\
							qui correspond à cette raison suivante :\n\
						</div>\n\
						<div style=\"font-family: Helvetica; font-size: 150%; font-weight: bold\">\n\
							<br>" + g_statusCode[_statusCode] + "\n\
						</div>\n\
					</center>\n\
				</div>\n\
			</body>\n\
		</html>\n";
	return (errorPage);
}

std::string		Response::_generateFormAcceptPage()
{
	std::string	htmlPage;

	htmlPage = "<!DOCTYPE html>\n\
		<html>\n\
			<head>\n\
				<title>Cutie webserv 😚 💖</title>\n\
				<meta charset=\"UTF-8\">\n\
				<link href=\"form_accept.css\" rel=\"stylesheet\">\n\
				<link rel=\"icon\" href=\"../img/favicon.ico\">\n\
			</head>\n\
			<body class=\"background\">\n\
				<div class=\"icon\">\n\
					<a href=\"form_order\"><img src=\"img/icon_panier_no_cgi.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"icon_cgi\">\n\
					<a href=\"cgi/cgi_achat.php\"><img src=\"img/icon_panier_with_cgi.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"paint\">\n\
					<a href=\"form_gallery\"><img src=\"img/icon_paint.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"house\">\n\
					<a href=\"index.html\"><img src=\"img/icon_house.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"danse\">\n\
					<a href=\"http://gcasale.free.fr/viva.htm\"><img src=\"img/icon_dance.gif\" width=\"90px\"></a>\n\
				</div>\n\
				<h1>\n\
					<center>Demande d'adoption ACCEPTED 💝</center>\n\
				</h1>\n\
				<div class=\"hamster\">\n\
					<img src=\"img/hamster_cookie.jpg\" width=\"640\">\n\
				</div>\n\
				<div class=\"requete\">\n\
					<center>Votre requete pour obtenir un hamster a ete accepte.<br></center>\n\
				</div>\n\
				<div class=\"happy\">\n\
					<center>Vous avez rendu un hamster heureux !</center>\n\
				</div>\n\
					<form action=\"./form.html\">\n\
						<center><input type=\"submit\" value=\"Creer une nouvelle demande pour creer une horde d'hamsters\" class=\"bouton\" /></center>\n\
					</form>\n\
					<form action=\"form_order\">\n\
						<center><input type=\"submit\" value=\"Voir le contenu du panier\" class=\"bouton\" /></center>\n\
					</form>\n\
					<form action=\"./index.html\">\n\
						<center><input type=\"submit\" value=\"Retour vers la page d'accueil\" class=\"bouton\" /></center>\n\
					</form>\n\
				</div>\n\
			</body>\n\
		</html>\n";
	return (htmlPage);
}

std::string		Response::_generateFormOrderPage()
{
	std::string						htmlPage;
	listOfPurchases					order;
	listOfPurchases::const_iterator	ite;

	order = _session->getOrder();
	htmlPage = "<!DOCTYPE html>\n\
		<html>\n\
			<head>\n\
				<title>Cutie webserv 😚 💖</title>\n\
				<meta charset=\"UTF-8\">\n\
				<link href=\"form_order.css\" rel=\"stylesheet\">\n\
				<link rel=\"icon\" href=\"../img/favicon.ico\">\n\
			</head>\n\
			<body class=\"background\">\n\
				<div class=\"icon\">\n\
					<a href=\"cgi/cgi_achat.php\"><img src=\"img/icon_panier_with_cgi.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"house\">\n\
					<a href=\"index.html\"><img src=\"img/icon_house.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"paint\">\n\
					<a href=\"form_gallery\"><img src=\"img/icon_paint.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"danse\">\n\
					<a href=\"http://gcasale.free.fr/viva.htm\"><img src=\"img/icon_dance.gif\" width=\"90px\"></a>\n\
				</div>\n\
				<h1>\n\
					Etat de votre panier 🛒\n\
				</h1>\n\
				<p class=\"cgi\">\n\
					sans CGI !\n\
				</p>\n\
				<div class=\"informations\">\n";

	for (ite = order.begin(); ite != order.end(); ite++)
	{
		htmlPage += \
				"<div class=\"name\">\n\
					Votre Nom : " + ite->getName() + "<br>\n\
				</div>\n\
				<div class=\"hamster\">\n\
					Votre Hamster : " + ite->getHamster() + "<br>\n\
				</div>\n\
				<div class=\"color\">\n\
					Sa Fourrure : " + ite->getColor() + "<br>\n\
				</div>\n\
				<div>\n\
					<button class=\"bouton\" id=\"" + ite->getId() + "\" onclick=\"deleteData(this.id)\">Jeter votre hamster et reformuler une adoption</button>\n\
				</div>\n\n\n";
	}

	htmlPage += \
				"\n\n\
				</div>\n\
				<form action=\"/form_sans_cgi.html\">\n\
					<input type=\"submit\" value=\"Go acheter un nouvel ami !\" class=\"bouton\" />\n\
				</form>\n\
				<form action=\"/index.html\">\n\
					<input type=\"submit\" value=\"Retour vers la page d'accueil\" class=\"bouton\" />\n\
				</form>\n\
				</div>\n\
				<script>\n\
					function deleteData(id) {\n\
						alert('The selected item will be deleted');\n\
						fetch(`/form_delete/${id}`, {\n\
							method: 'DELETE',\n\
							credentials: 'include'\n\
						})\n\
						.then(response => {\n\
							window.location.reload();\n\
						})\n\
					}\n\
				</script>\n\
			</body>\n\
		</html>\n";
	return (htmlPage);
}

std::string		Response::_generateGalleryPage()
{
	std::string									htmlPage;
	//std::vector<std::string>					gallery;
	typedef std::vector<std::pair<std::string, std::string> > listOfPath;
	//std::vector<std::pair<std::string, std::string> >		gallery;
	listOfPath									gallery;
	listOfPath::const_iterator					ite;
	//std::vector<std::string>::const_iterator	ite;

	gallery = _session->getGallery();
	htmlPage = "<!DOCTYPE html>\n\
		<html>\n\
			<head>\n\
				<title>Cutie webserv 😚 💖</title>\n\
				<meta charset=\"UTF-8\">\n\
				<link href=\"form_gallery.css\" rel=\"stylesheet\">\n\
				<link rel=\"icon\" href=\"../img/favicon.ico\">\n\
			</head>\n\
			<body bgcolor=\"#fff8e7\">\n\
				<div class=\"icon\">\n\
					<a href=\"form_order\"><img src=\"img/icon_panier_no_cgi.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"icon_cgi\">\n\
					<a href=\"cgi/cgi_achat.php\"><img src=\"img/icon_panier_with_cgi.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"house\">\n\
					<a href=\"index.html\"><img src=\"img/icon_house.png\" width=\"100px\"></a>\n\
				</div>\n\
				<div class=\"danse\">\n\
					<a href=\"http://gcasale.free.fr/viva.htm\"><img src=\"img/icon_dance.gif\" width=\"90px\"></a>\n\
				</div>\n\
				<h1>\n\
					<center>🪞 Galerie de vos plus beaux hamsters 🪞</center>\n\
				</h1>\n";

	if (gallery.empty()) {
		htmlPage += \
					"<div class=\"empty\">\n\
						<center>Aucune image dans la galerie...</center>\n\
					</div>\n";
	} else {
		for (ite = gallery.begin(); ite != gallery.end(); ite++) {
			htmlPage += \
					"<div class=\"hamster\">\n\
						<img src=" + ite->second + " width=\"500\">\n\
					</div>\n\
					<form action=\"\">\n\
						<center><button class=\"bouton\" id=\"" + ite->second + "\" onclick=\"deleteData(this.id)\">Enveler cette horreur de la galerie</button><center>\n\
					</form>\n";
		}
	}

	htmlPage += "<script>\n\
					function deleteData(id) {\n\
						alert('The selected item will be deleted');\n\
						fetch(`/form_delete${id}`, {\n\
							method: 'DELETE',\n\
							credentials: 'include'\n\
						})\n\
						.then(response => {\n\
							window.location.reload();\n\
						})\n\
					}\n\
				</script>\n\
				</div>\n\
				<form action=\"form_upload.html\">\n\
					<center><input type=\"submit\" value=\"Ajouter une œuvre d'art\" class=\"bouton\" /></center>\n\
				</form>\n\
			</body>\n\
		</html>\n";

	return (htmlPage);
}
