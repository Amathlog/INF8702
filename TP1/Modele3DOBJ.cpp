///////////////////////////////////////////////////////////////////////////////
///  @file Mod�le3D-OBJ.cpp
///  @author  Fr�d�ric Plourde
///  @brief   D�finit la classe CModele3DOBJ qui encapsule un modele3D 
///           charg� � partir d'un fichier OBJ
///  @date    2008-10-27
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Modele3DOBJ.h"

//vector<Vertex>    CModele3DOBJ::g_Vertices_;
//vector<Triangle>  CModele3DOBJ::g_Triangles_;
//vector<Quad>      CModele3DOBJ::g_Quads_;
//CVecteur3         CModele3DOBJ::g_ObjCentroid_;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CModele3DOBJ \n
///  constructeur d'un mod�le 3D avec 1 seule texture (utile seulement pour 
///  faire des tests du c�t� openGL... pas utile pour le labo en tant que tel...
///
///  @param [in, out]  nomsFichierOBJ char *	    La cha�ne de caract�res contenant le nom de fichier OBJ du mod�le 3D
///  @param [in, out]  nomFichierTexture char *     l'unique nom de fichier de l'unique texture
///  @param [in]       echelle float [=1.000000]    mise � l'�chelle qui sera appliqu�e automatique (glScale)
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-10-27
///
///////////////////////////////////////////////////////////////////////////////
CModele3DOBJ::CModele3DOBJ(char* nomFichierOBJ, char * nomFichierTexture, float echelle) :
CModele3DAbstrait(echelle, nomFichierTexture, TYPE_TEXTURE2D, true)
{
	// charger le mod�le 3D en m�moire
	chargerModeleOBJ(nomFichierOBJ);
}


///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CModele3DOBJ \n
///  constructeur d'un mod�le 3D avec plusieurs textures. Cette version du constructeur
///  est particuli�rement utile lorsqu'on veut associer plusieurs textures aux diff�rentes
///  unit�es de texture du nuanceur.
///
///  @param [in, out]  nomsFichierOBJ char *        La cha�ne de caract�res contenant le nom de fichier OBJ du mod�le 3D
///  @param [in, out]  nomsFichiersTextures std::vector<char*> *    Le vecteur contenant la liste des noms de fichiers. Cette liste est copi�e, alors l'appelant peut en disposer.
///  @param [in]       echelle float [=1.000000]    mise � l'�chelle qui sera appliqu�e automatique (glScale)
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-10-27
///
///////////////////////////////////////////////////////////////////////////////
CModele3DOBJ::CModele3DOBJ(char* nomFichierOBJ, std::vector<char*> *nomsFichiersTextures, float echelle) :
CModele3DAbstrait(echelle, nomsFichiersTextures, TYPE_TEXTURE2D, true)
{
	// charger le mod�le 3D en m�moire
	chargerModeleOBJ(nomFichierOBJ);
}



///////////////////////////////////////////////////////////////////////////////
///  public destructor  ~CModele3DOBJ \n
///  Destructeur par d�faut.
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-10-27
///
///////////////////////////////////////////////////////////////////////////////
CModele3DOBJ::~CModele3DOBJ(void)
{
	// Clean up
	g_Vertices_.clear();
	g_Triangles_.clear();
	g_Quads_.clear();
}


///////////////////////////////////////////////////////////////////////////////
///  public  chargerModeleOBJ \n
///  Fonction qui charge en m�moire les sommets et les normales d'un mod�le 
///  d�finit dans un fichier .OBJ.
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-10-27
///
///////////////////////////////////////////////////////////////////////////////
void CModele3DOBJ::chargerModeleOBJ(char* fichierOBJ)
{
	COBJParser* Parser = new COBJParser();

	if (Parser->OpenFile(fichierOBJ))
	{
		GLenum error = glGetError();
		cerr << "Lecture du fichier : " << fichierOBJ << endl;
		Parser->ParseFile();
		Parser->CloseFile();

		g_Vertices_ = Parser->GetVertices();
		g_Triangles_ = Parser->GetTriangles();
		g_Quads_ = Parser->GetQuads();
		g_ObjCentroid_ = Parser->GetCentroid();

		cerr << "Centroid: " << g_ObjCentroid_ << endl;
		cerr << "Nombre de faces : " << (g_Triangles_.size() + g_Quads_.size()) << endl;

		vector<float> vertex_vector = getFloatsFromVertices(g_Vertices_);
		int byte_size = vertex_vector.size() * sizeof(float);


		// Incomplet lors du 1er labo...
		// Manque code VBO, IBO, VAO

		cerr << "Chargement termine !!" << endl;
	}

	delete Parser;
}


///////////////////////////////////////////////////////////////////////////////
///  public static  afficherModele3DOGL4 \n
///  Fonction qui affiche le mod�le 3D � partir des tableaux de sommets lus 
///  dans le fichier source OBJ.
///
///  @return Aucune
///
///  @author F�lix G. Harvey
///  @date   2016
///
///////////////////////////////////////////////////////////////////////////////
void CModele3DOBJ::dessiner()
{
	// Incomplet pour 1er labo ...
	
}

