///////////////////////////////////////////////////////////////////////////////
///  @file Texture2D.cpp
///  @author  Fr�d�ric Plourde 
///  @brief   D�finit la classe CTexture2D qui impl�mente une texture2D openGL
///  @date    2008-01-18
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#include "Texture2D.h"

///////////////////////////////////////////////////////////////////////////////
///  public constructor  CTexture2D \n
///  Construit une texture simple 2D (de type GL_TEXTURE_2D). Construit aussi
///  une texture de base abstraite
///
///  @param [in, out]  filename char *    le nom de fichier de la texture 2D
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CTexture2D::CTexture2D(char* filename, bool mipmap):
CTextureAbstraite(filename, GL_TEXTURE_2D, mipmap)
{
   chargerTexture();
}


///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTexture2D \n
///  Construit une texture simple 2D (de type GL_TEXTURE_2D) � partir des donn�es
///  brutes de type float (format RGB) et (type GL_FLOAT)
///
///  @param [in, out]  data vect3D *  le pointeur sur les donn�es float R-G-B-R-G-B...
///  @param [in]       hauteur int    la hauteur de la texture
///  @param [in]       largeur int    la largeur de la texture
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-03-09
///
///////////////////////////////////////////////////////////////////////////////
CTexture2D::CTexture2D(vect3D* data, int hauteur, int largeur, bool mipmap):
CTextureAbstraite(GL_TEXTURE_2D, mipmap)
{
   // aiguiller openGL vers la zone m�moire contenant le graphisme de la texture
   // Futur TP...
}


///////////////////////////////////////////////////////////////////////////////
///  public destructor  ~CTexture2D \n
///  Destructeur par d�faut d'une texture 2D
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CTexture2D::~CTexture2D(void)
{
}



///////////////////////////////////////////////////////////////////////////////
///  private virtual  chargerTexture \n
///  Charge en m�moire d'application la texture sp�cifi�e au niveau d'openGL.
///
///  @return Aucune
///
///  @author Fr�d�ric Plourde 
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
void CTexture2D::chargerTexture()
{
   // Futur TP...
}