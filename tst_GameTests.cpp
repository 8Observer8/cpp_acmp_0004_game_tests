#include <QString>
#include <QtTest>
#include <stdexcept>
#include <sstream>

class LogicError : public std::logic_error {
public:

    LogicError( int argument ) : std::logic_error( "" ), m_argument( argument ) {

    }

    virtual const char *what( ) const throw () {
        return m_message.c_str( );
    }

    virtual ~LogicError( ) throw () {

    }

protected:
    int m_argument;
    std::string m_message;
};

class OutOfRange : public LogicError {
public:

    OutOfRange( int argument, int beginOfRange, int endOfRange ) : LogicError( argument ) {
        std::string str_argument, str_beginOfRange, str_endOfRange;

        str_argument = intToString( argument );
        str_beginOfRange = intToString( beginOfRange );
        str_endOfRange = intToString( endOfRange );

        m_message = "Argument " + str_argument + " doen't hit in the range [" +
                str_beginOfRange + ", " + str_endOfRange + "]";
    }

private:

    std::string intToString( int number ) {
        std::stringstream stream;
        stream << number;
        return stream.str( );
    }
};

int game( int number ) throw (OutOfRange);

class GameTests : public QObject {
    Q_OBJECT

public:
    GameTests( );

    private
Q_SLOTS:
    void testCase1_data( );
    void testCase1( );
};

GameTests::GameTests( ) {
}

void GameTests::testCase1_data( ) {
    QTest::addColumn<int>("number");
    QTest::addColumn<int>("expected");
    QTest::addColumn<bool>("isExpectedException");

    bool exception = true;
    bool no_exception = false;

    QTest::newRow( "game_01" ) << -1 << 0 << exception;
    QTest::newRow( "game_02" ) << 10 << 0 << exception;
    QTest::newRow( "game_03" ) << 0 << 99 << no_exception;
    QTest::newRow( "game_04" ) << 1 << 198 << no_exception;
    QTest::newRow( "game_05" ) << 8 << 891 << no_exception;
    QTest::newRow( "game_06" ) << 9 << 990 << no_exception;
}

void GameTests::testCase1( ) {
    QFETCH( int, number );
    QFETCH( int, expected );
    QFETCH( bool, isExpectedException );

    try {
        int actual = game( number );
        if ( isExpectedException ) {
            QVERIFY2( false, "There is no exception." );
        } else {
            QCOMPARE( actual, expected );
        }
    } catch ( const LogicError& e ) {
        if ( !isExpectedException ) {
            QVERIFY2( false, "Exception was occur." );
        } else {
            QVERIFY2( true, "" );
        }
    } catch ( ... ) {
        QVERIFY2( false, "Uncaught exception." );
    }

}

QTEST_APPLESS_MAIN( GameTests )

#include "tst_GameTests.moc"
