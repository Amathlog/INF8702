#include "Quadrique.h"

using namespace Scene;

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CQuadrique \n
///  Description : Constructeur par d�faut
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique( void )
    : ISurface     (                  )
    , m_Quadratique( CVecteur3::ZERO  )
    , m_Lineaire   ( CVecteur3::ZERO  )
    , m_Mixte      ( CVecteur3::ZERO  )
    , m_Cst        ( RENDRE_REEL( 0 ) )
{}

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CQuadrique \n
///  Description : Constructeur par d�faut
///
///  @param [in]       Quadric const Scene::CQuadrique &   la quadrique � copier
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::CQuadrique( const CQuadrique& Quadric )
    : ISurface     ( Quadric               )
    , m_Quadratique( Quadric.m_Quadratique )
    , m_Lineaire   ( Quadric.m_Lineaire    )
    , m_Mixte      ( Quadric.m_Mixte       )
    , m_Cst        ( Quadric.m_Cst         )
{}

///////////////////////////////////////////////////////////////////////////////
///  public virtual destructor  ~CQuadrique \n
///  Description : Destructeur
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique::~CQuadrique( void )
{
}

///////////////////////////////////////////////////////////////////////////////
///  public  operator = \n
///  Description : Op�rateur de copie
///
///  @param [in]       Quadric const Scene::CQuadrique &    La quadrique � copier
///
///  @return Scene::CQuadrique & La quadrique modifi�e
///
///  @author Olivier Dionne 
///  @date   14/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique& CQuadrique::operator = ( const CQuadrique& Quadric )
{
    ISurface::operator =( Quadric );
    m_Quadratique = Quadric.m_Quadratique;
    m_Lineaire    = Quadric.m_Lineaire;
    m_Mixte       = Quadric.m_Mixte;
    m_Cst         = Quadric.m_Cst;
    return ( *this );
}

///////////////////////////////////////////////////////////////////////////////
///  protected virtual constant  AfficherInfoDebug \n
///  Description : Impl�mente le d�boguage polymorphique par flux de sortie
///
///  @param [in, out]  Out std::ostream &    Le flux de sortie
///
///  @return std::ostream & Le flux de sortie modifi�
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
ostream& CQuadrique::AfficherInfoDebug( ostream& Out ) const
{
    Out << "[DEBUG]: Quadric.Quadratique       = " << m_Quadratique << endl;
    Out << "[DEBUG]: Quadric.Lineaire          = " << m_Lineaire    << endl;
    Out << "[DEBUG]: Quadric.Mixte             = " << m_Mixte       << endl;
    Out << "[DEBUG]: Quadric.Constante         = " << m_Cst;
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Pretraitement \n
///  Description : Pretraitement des donn�es de la quadrique( appel� AVANT le lancer)
///
///  @return None
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
void CQuadrique::Pretraitement( void )
{
   // Algorithme tir� de ... 
   // R. Goldman, "Two Approach to a Computer Model for Quadric Surfaces",
   // IEEE CG&A, Sept 1983, pp.21
   
    REAL A = m_Quadratique.x;
	REAL B = m_Quadratique.y;
    REAL C = m_Quadratique.z;
    REAL D = m_Mixte.z    * RENDRE_REEL( 0.5 );
	REAL E = m_Mixte.x    * RENDRE_REEL( 0.5 );
	REAL F = m_Mixte.y    * RENDRE_REEL( 0.5 );
	REAL G = m_Lineaire.x * RENDRE_REEL( 0.5 );
	REAL H = m_Lineaire.y * RENDRE_REEL( 0.5 );
	REAL J = m_Lineaire.z * RENDRE_REEL( 0.5 );
	REAL K = m_Cst;

	CMatrice4 Q( A, D, F, G,
			     D, B, E, H,
			     F, E, C, J,
			     G, H, J, K );

    CMatrice4 Inverse = m_Transformation.Inverse();

    Q = Inverse * Q * Inverse.Transpose();

    m_Quadratique.x = Q[ 0 ][ 0 ];
    m_Quadratique.y = Q[ 1 ][ 1 ];
	m_Quadratique.z = Q[ 2 ][ 2 ];
	m_Cst           = Q[ 3 ][ 3 ];
	m_Mixte.x       = Q[ 1 ][ 2 ] * RENDRE_REEL( 2.0 );
	m_Mixte.y       = Q[ 0 ][ 2 ] * RENDRE_REEL( 2.0 );
	m_Mixte.z       = Q[ 0 ][ 1 ] * RENDRE_REEL( 2.0 );
	m_Lineaire.x    = Q[ 0 ][ 3 ] * RENDRE_REEL( 2.0 );
	m_Lineaire.y    = Q[ 1 ][ 3 ] * RENDRE_REEL( 2.0 );
    m_Lineaire.z    = Q[ 2 ][ 3 ] * RENDRE_REEL( 2.0 );

}

///////////////////////////////////////////////////////////////////////////////
///  public virtual  Intersection \n
///  Description : Effectue l'intersection Rayon/Quadrique
///
///  @param [in]       Rayon const CRayon &    Le rayon � tester
///
///  @return Scene::CIntersection Le r�sultat de l'ntersection
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CIntersection CQuadrique::Intersection( const CRayon& Rayon )
{
	CIntersection Result;

	// Coefficients de notre rayon
	REAL x0 = Rayon.ObtenirOrigine().x;
	REAL y0 = Rayon.ObtenirOrigine().y;
	REAL z0 = Rayon.ObtenirOrigine().z;

	CVecteur3 direction = Rayon.ObtenirDirection();
	REAL xd = direction.x;
	REAL yd = direction.y;
	REAL zd = direction.z;

	// Coefficients de la matrice
	REAL A = m_Quadratique.x;
	REAL E = m_Quadratique.y;
	REAL H = m_Quadratique.z;
	REAL B = m_Mixte.z;
	REAL F = m_Mixte.x;
	REAL C = m_Mixte.y;
	REAL D = m_Lineaire.x;
	REAL G = m_Lineaire.y;
	REAL I = m_Lineaire.z;
	REAL J = m_Cst;

	// Coefficients d'intersection
	REAL aq = A*xd*xd + RENDRE_REEL(2.0) * B*xd*yd + RENDRE_REEL(2.0) * C*xd*zd + E*yd*yd + RENDRE_REEL(2.0) * F*yd*zd + H*zd*zd;
	REAL bq = RENDRE_REEL(2.0) * A*x0*xd + RENDRE_REEL(2.0) * B*(x0*yd + xd*y0) + RENDRE_REEL(2.0) * C*(x0*zd + xd*z0) + D*xd + RENDRE_REEL(2.0) * E*y0*yd + RENDRE_REEL(2.0) * F*(y0*zd + yd*z0) + G*yd + RENDRE_REEL(2.0) * H*z0*zd + I*zd;
	REAL cq = A * x0*x0 + RENDRE_REEL(2.0) * B*x0*y0 + RENDRE_REEL(2.0) * C*x0*z0 + D*x0 + E*y0*y0 + RENDRE_REEL(2.0) * F*y0*z0 + G*y0 + H*z0*z0 + I*z0 + J;

	REAL t0;
	if (aq == RENDRE_REEL(0.0)) {
		t0 = -cq / bq;
	} else {
		// Discriminant
		REAL delta = bq*bq - RENDRE_REEL(4.0) * aq * cq;
		if (delta < RENDRE_REEL(0.0)) {
			// Pas d'intersection
			return Result;
		}
		REAL sqrtDelta = sqrt(delta);

		// Calcul des racines
		REAL t1;
		t0 = (-bq + sqrtDelta) / (RENDRE_REEL(2.0) * aq);
		t1 = (-bq - sqrtDelta) / (RENDRE_REEL(2.0) * aq);
		if (t0 <= RENDRE_REEL(0.0) && t1 <= RENDRE_REEL(0.0)) {
			return Result;
		} else if (t0 <= RENDRE_REEL(0.0) && t1 > RENDRE_REEL(0.0)) {
			t0 = t1;
		} else if (t0 > RENDRE_REEL(0.0) && t1 <= RENDRE_REEL(0.0)) {
			// t0 = t0
		} else {
			t0 = Min(t0, t1);
		}
	}

	// Point d'intersection et normale
	CVecteur3 inter(Rayon.ObtenirOrigine() + t0 * Rayon.ObtenirDirection());
	CVecteur3 normale(RENDRE_REEL(2.0) * (A * inter.x + B * inter.y + C * inter.z) + D,
		RENDRE_REEL(2.0) * (B * inter.x + E * inter.y + F * inter.z) + G,
		RENDRE_REEL(2.0) * (C * inter.x + F * inter.y + H * inter.z) + I);
	normale = CVecteur3::Normaliser(normale);

	Result.AjusterSurface(this);
	Result.AjusterDistance(CVecteur3::Distance(inter, Rayon.ObtenirOrigine()));
	Result.AjusterNormale((CVecteur3::ProdScal(normale, Rayon.ObtenirDirection()) > RENDRE_REEL(0.0)) ? -normale : normale);
	
	return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  public virtual constant  Copier \n
///  Description : Alloue une copie de la quadrique courante
///
///  @return Scene::CQuadrique *la copie de la quadrique
///
///  @author Olivier Dionne 
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
CQuadrique* CQuadrique::Copier( void ) const
{
    return new CQuadrique( *this );
}