#pragma once
class GCSquarePeople;

class GCSquarePeopleElesis
{
public:
    GCSquarePeopleElesis(void) {}
    virtual ~GCSquarePeopleElesis(void) {}

    static void OnWait( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnWalk( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnDash( GCSquarePeople* pPeople, bool bKeyProc );

    static void OnJumpRemote( GCSquarePeople* pPeople );
    static void OnDashRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleElesis2
{
public:
    GCSquarePeopleElesis2(void) {}
    virtual ~GCSquarePeopleElesis2(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleElesis3
{
public:
    GCSquarePeopleElesis3(void) {}
    virtual ~GCSquarePeopleElesis3(void) {}

    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleElesis4
{
public:
    GCSquarePeopleElesis4(void) {}
    virtual ~GCSquarePeopleElesis4(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleLire3
{
public:
    GCSquarePeopleLire3(void) {}
    virtual ~GCSquarePeopleLire3(void) {}

    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleLire4
{
public:
    GCSquarePeopleLire4(void) {}
    virtual ~GCSquarePeopleLire4(void) {}

    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleArme
{
public:
    GCSquarePeopleArme(void) {}
    virtual ~GCSquarePeopleArme(void) {}

    static void OnWait( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnWaitRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleArme2
{
public:
    GCSquarePeopleArme2(void) {}
    virtual ~GCSquarePeopleArme2(void) {}

    static void OnWait( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnWaitRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleArme3
{
public:
    GCSquarePeopleArme3(void) {}
    virtual ~GCSquarePeopleArme3(void) {}

    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleArme4
{
public:
    GCSquarePeopleArme4(void) {}
    virtual ~GCSquarePeopleArme4(void) {}

    static void OnWait( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
    static void OnWaitRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleLas
{
public:
    GCSquarePeopleLas(void) {}
    virtual ~GCSquarePeopleLas(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleLas2
{
public:
    GCSquarePeopleLas2(void) {}
    virtual ~GCSquarePeopleLas2(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleLas3
{
public:
    GCSquarePeopleLas3(void) {}
    virtual ~GCSquarePeopleLas3(void) {}

    static void OnJump( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnJumpRemote( GCSquarePeople* pPeople );
};

class GCSquarePeopleRyan2
{
public:
    GCSquarePeopleRyan2(void) {}
    virtual ~GCSquarePeopleRyan2(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleRyan3
{
public:
    GCSquarePeopleRyan3(void) {}
    virtual ~GCSquarePeopleRyan3(void) {}

    static void OnJumpReady( GCSquarePeople* pPeople, bool bKeyProc );
};

class GCSquarePeopleLey1
{
public:
    GCSquarePeopleLey1(void) {}
    virtual ~GCSquarePeopleLey1(void){}

    static void OnDash( GCSquarePeople* pPeople, bool bKeyProc );
    static void OnDashRemote( GCSquarePeople* pPeople );
    //static void OnJumpRemote( GCSquarePeople* pPeople );
};