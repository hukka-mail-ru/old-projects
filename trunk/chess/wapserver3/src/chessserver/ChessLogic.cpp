#include "ChessLogic.h"

ChessLogic::ChessLogic()
{
}

ChessLogic::~ChessLogic()
{
}

ChessGameStatus ChessLogic::GetStatus()
{
    return m_Engine.getResult();
}

bool ChessLogic::SetPos( const TVByte &_vecBytes )
{
    return m_Engine.setPosition( &_vecBytes );
}

IGameLogic::StepRes ChessLogic::StepAnl( TVByte *_vecBytes)
{
    if ( _vecBytes->size() > 5 )
    {
   //     cerr << "ERR    ChessLogic::StepAnl _vecBytes->size() < 5" << endl;
        return NotValid;
    }

    String strMove = "";

    for ( TVByte::const_iterator i = _vecBytes->begin(); i != _vecBytes->end(); ++i )
    {
        strMove += *i;
    }

    if ( !m_Engine.move( strMove ) )
    {
    //    cerr << "ERR    ChessLogic::StepAnl !m_Engine.move" << endl;
        return NotValid;
    }

    if ( m_Engine.getResult() == Checkmate )
    {
    	return CheckMate;
    }
    else if ( m_Engine.getResult() == Stalemate ||
              m_Engine.getResult() == FiftyMoves ||
              m_Engine.getResult() == TripleOccurrence )
    {
        return Draw;
    }

    return Valid;
}

const TVByte* ChessLogic::GetPosForDB()
{
    return m_Engine.getPosition();
}

const TVByte* ChessLogic::GetPosForClient()
{
    /*
    TVByte::const_iterator i = m_Engine.getPositionForClient()->begin();
    m_vecbtPosForClient.clear();

    for ( int n = 0; n < 64 + 5; ++n )
    {
        m_vecbtPosForClient.push_back( *i++ );
    }

    return &m_vecbtPosForClient;
  */
    m_vecbtPosForClient = *m_Engine.getPositionForClient();

    return &m_vecbtPosForClient;



}
