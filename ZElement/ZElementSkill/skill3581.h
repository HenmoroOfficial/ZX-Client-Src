#ifndef __CPPGEN_GNET_SKILL3581
#define __CPPGEN_GNET_SKILL3581
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3581:public Skill
    {
      public:
        enum
        { SKILL_ID = 3581 };
          Skill3581 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3581Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3581Stub ():SkillStub (3581)
        {
            occupation = 169;
            name = L"∑Ÿœ„”Ò≤·<Ï¯>";
            nativename = "∑Ÿœ„”Ò≤·<Ï¯>";
            icon = "∑°§∑Ÿœ„”Ò≤·.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 4;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 9;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 3404;
            talent_size = 1;
            action[0] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[1] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[2] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[3] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[4] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[5] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[6] = "∑Ÿœ„π»_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[16] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[17] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[18] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[19] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[20] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[21] = "∑Ÿœ„π»_∆Ô≥À_∑Ÿœ„”Ò≤·°æ”Ò—Ù°ø";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3581Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 32 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3453 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500 + 10 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 20000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               1500 + 10 * skill->GetLevel () * skill->GetLevel (),
                               200 + 32 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               120,
                               2 + int (skill->GetLevel () / 4),
                               5 + 3 * int (skill->GetLevel () / 5), 20 + skill->GetLevel (), 6 + int (skill->GetLevel () / 5));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->
                SetTime ((5 + 3 * int (skill->GetLevel () / 5) +
                          ((2 + int (skill->GetLevel () / 4))>skill->GetPlayer ()->GetDebuffcnt ()? skill->GetPlayer ()->GetDebuffcnt () : (2 +
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetLevel
                                                                                                                                             () /
                                                                                                                                             4))) *
                          2) *1000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (2 + int (skill->GetLevel () / 4));
              skill->GetVictim ()->SetTurndebuff (1);
              return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (120000 - 20000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (3453);
            skill->GetVictim ()->SetAmount (3453);
            skill->GetVictim ()->SetValue (3453);
            skill->GetVictim ()->SetSetcooldown (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->
                SetTime ((5 + 3 * int (skill->GetLevel () / 5) +
                          ((2 + int (skill->GetLevel () / 4))>skill->GetPlayer ()->GetDebuffcnt ()? skill->GetPlayer ()->GetDebuffcnt () : (2 +
                                                                                                                                            int
                                                                                                                                            (skill->
                                                                                                                                             GetLevel
                                                                                                                                             () /
                                                                                                                                             4))) *
                          2) *1000);
            skill->GetVictim ()->SetRatio (0.01 *
                                           (20 + skill->GetLevel () +
                                            ((2 +
                                              int (skill->GetLevel () /
                                                   4)) >skill->GetPlayer ()->GetDebuffcnt ()? skill->GetPlayer ()->GetDebuffcnt () : (2 +
                                                                                                                                      int (skill->
                                                                                                                                           GetLevel ()
                                                                                                                                           / 4))) *
                                            (6 + int (skill->GetLevel () / 5))));
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetRetort (1);
            return true;
        }
#endif
    };
}
#endif
