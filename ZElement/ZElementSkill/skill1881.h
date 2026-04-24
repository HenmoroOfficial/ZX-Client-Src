#ifndef __CPPGEN_GNET_SKILL1881
#define __CPPGEN_GNET_SKILL1881
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1881:public Skill
    {
      public:
        enum
        { SKILL_ID = 1881 };
          Skill1881 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1881Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1881Stub ():SkillStub (1881)
        {
            occupation = 37;
            name = L"ò¿»ê";
            nativename = "ò¿»ê";
            icon = "ò¿»ê.dds";
            maxlevel = 3;
            maxlearn = 2;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 5;
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
            talent[0] = 2101;
            talent[1] = 2947;
            talent[2] = 4112;
            talent[3] = 4530;
            talent_size = 4;
            action[0] = "ÊÞÉñ_ò¿»ê";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "ÊÞÉñ_ò¿»ê";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÊÞÉñ_Æï³Ë_ò¿»ê";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_ò¿»ê";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
            doenchant = true;
            dobless = false;
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
        virtual ~ Skill1881Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1879 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (900);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 * 3 - 20000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[2] = { 140, 141 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               5 * skill->GetLevel (),
                               5 * skill->GetLevel (),
                               12 + 12 * skill->GetLevel (),
                               skill->GetT1 () > 0 || skill->GetT2 () > 0 || skill->GetT3 () > 0 ? table.Find (18813) : table.Find (18814));

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (12000 + 12000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.05 * skill->GetLevel () +
                                            (skill->GetT1 () >=
                                             skill->GetT2 ()? (skill->GetT1 () >=
                                                               skill->GetT3 ()? skill->GetT1 () : skill->GetT3 ()) : (skill->GetT2 () >=
                                                                                                                      skill->GetT3 ()? skill->
                                                                                                                      GetT2 () : skill->GetT3 ())) *
                                            0.012);
            skill->GetVictim ()->SetValue (0.05 * skill->GetLevel () +
                                           (skill->GetT1 () >=
                                            skill->GetT2 ()? (skill->GetT1 () >=
                                                              skill->GetT3 ()? skill->GetT1 () : skill->GetT3 ()) : (skill->GetT2 () >=
                                                                                                                     skill->GetT3 ()? skill->
                                                                                                                     GetT2 () : skill->GetT3 ())) *
                                           0.012);
            skill->GetVictim ()->SetChihun (1);
            return true;
        }
#endif
    };
}
#endif
