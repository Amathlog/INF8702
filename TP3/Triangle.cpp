#include "Triangle.h"
#include "Plan.h"

using namespace Scene;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTriangle \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle( void )
    : ISurface (                 )
    , m_Normale( CVecteur3::ZERO )
{}

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CTriangle \n
///  Description : Constructeur par défaut
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::CTriangle( const CTriangle& Triangle )
    : ISurface ( Triangle           )
    , m_Normale( Triangle.m_Normale )
{
    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = Triangle.m_Pts[ i ];
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual destructor  ~CTriangle \n
///  Description : Destructeur
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle::~CTriangle( void )
{
}

///////////////////////////////////////////////////////////////////////////////
///  public  operator = \n
///  Description : Opérateur de copie
///
///  @param [in]       Triangle const Scene::CTriangle &    Le triangle à copier
///
///  @return Scene::CTriangle & Le triangle modifié
///
///  @author Olivier Dionne 
///  @date   14/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle& CTriangle::operator = ( const CTriangle& Triangle )
{
    ISurface::operator =( Triangle );
    m_Normale = Triangle.m_Normale;

    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = Triangle.m_Pts[ i ];

    return ( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  protected virtual constant  AfficherInfoDebug \n
///  Description : Implémente le déboguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &   Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
ostream& CTriangle::AfficherInfoDebug( ostream& Out ) const
{
    Out << "[DEBUG]: Triangle.Point1         = " << m_Pts[ 0 ] << endl;
    Out << "[DEBUG]: Triangle.Point2         = " << m_Pts[ 1 ] << endl;
    Out << "[DEBUG]: Triangle.Point3         = " << m_Pts[ 2 ] << endl;
    Out << "[DEBUG]: Triangle.Normale        = " << m_Normale;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Pretraitement \n
///  Description : Pretraitement des données du triangle ( Appelé AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CTriangle::Pretraitement( void )
{
    for( int i = 0; i < 3; i++ )
        m_Pts[ i ] = m_Pts[ i ] * m_Transformation;
    CalculerNormale();
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Intersection \n
///  Description : Effectue l'intersection Rayon/Triangle
///
///  @param [in]       Rayon const CRayon &    Le rayon à tester
///
///  @return Scene::CIntersection Le résultat de l'ntersection
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CIntersection CTriangle::Intersection(const CRayon& Rayon)
{
	CIntersection Result;

	// Calcul de l'équation du plan défini par le triangle ABC
	// n = (A - C) x (B - C)
	// cst = -(n.A)
	CVecteur3 normale = m_Normale;
	REAL cst = -CVecteur3::ProdScal(normale, m_Pts[0]);
	CPlan planTriangle;
	planTriangle.AjusterNormale(normale);
	planTriangle.AjusterConstante(cst);

	// Résolution de l'intersection avec le plan
	CIntersection interPlan = planTriangle.Intersection(Rayon);

	// S'il n'y a pas d'intersection, on sort
	if (interPlan.ObtenirDistance() < EPSILON)
		return Result;

	// Calcul du point d'intersection P
	CVecteur3 ptInter = Rayon.ObtenirOrigine() + interPlan.ObtenirDistance() * Rayon.ObtenirDirection();

	// Calcul des coordonnées barycentriques, pour savoir si le point P est dans le triangle ABC
	// On cherche la solution au systeme w = su + tv, avec w=AP u=AB v=AC et s,t deux réels
	// Pour cela, on calcule le vecteur u' = n x u (produit vectoriel entre u et la normale au plan) et v' = n x v
	// D'où w.u' = su.u' + tv.u' = tv.u' (car u.u' = 0 car ils sont perpendiculaires).
	// On obtient donc t = (w.u')/(v.u') = (w.(n x u))/(v.(n x u))
	// De même s = (w.(n x v))/(u.(n x v))

	CVecteur3 u = m_Pts[1] - m_Pts[0]; // Vecteur AB
	CVecteur3 v = m_Pts[2] - m_Pts[0]; // Vecteur AC
	CVecteur3 w = ptInter  - m_Pts[0]; // Vecteur AP

	CVecteur3 uPrime = CVecteur3::ProdVect(normale, u);
	CVecteur3 vPrime = CVecteur3::ProdVect(normale, v);

	REAL s = CVecteur3::ProdScal(w, vPrime) / CVecteur3::ProdScal(u, vPrime);
	REAL t = CVecteur3::ProdScal(w, uPrime) / CVecteur3::ProdScal(v, uPrime);

	// Le point est à l'intérieur du triangle si s >= 0 && t >= 0 && s + t <= 1
	if (s >= 0.0 && t >= 0.0 && s + t <= 1.0) {
		Result.AjusterSurface(this);
		Result.AjusterDistance(interPlan.ObtenirDistance());
		Result.AjusterNormale(normale);
	}

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual constant  Copier \n
///  Description : Alloue une copie du triangle courant
///
///  @return Scene::CTriangle * Nouvelle copie du triangle 
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CTriangle* CTriangle::Copier( void ) const
{
    return new CTriangle( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  private  CalculerNormale \n
///  Description : Calculer la normale du triangle à partir des côtés
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CTriangle::CalculerNormale( void )
{
    CVecteur3 Edge1 = m_Pts[ 1 ] - m_Pts[ 0 ];
    CVecteur3 Edge2 = m_Pts[ 2 ] - m_Pts[ 0 ];
    m_Normale = CVecteur3::Normaliser( CVecteur3::ProdVect( Edge1, Edge2 ) );
}
