#ifndef __CPPGEN_GNET_SKILL2451
#define __CPPGEN_GNET_SKILL2451
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2451:public Skill
    {
      public:
        enum
        { SKILL_ID = 2451 };
          Skill2451 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2451Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrit (0.1 * skill->GetC0 ());
                skill->SetRatio (1.05 + 0.05 * skill->GetLevel () + 0.08 * skill->GetC0 () * skill->GetT1 () +
                                 0.012 * (skill->GetT2 () >=
                                          skill->GetT3 ()? (skill->GetT2 () >=
                                                            skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                   skill->GetT4 ()? skill->
                                                                                                                   GetT3 () : skill->GetT4 ())));
                skill->SetPlus (244 + 10 * skill->GetLevel () + skill->GetPlayer ()->GetHp () * 0.05 * skill->GetT0 ());
                skill->GetPlayer ()->SetHp (skill->GetPlayer ()->GetHp () * (1 - 0.05 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2451Stub ():SkillStub (2451)
        {
            occupation = 49;
            name = L"ÌìÖï";
            nativename = "ÌìÖï";
            icon = "ÌìÖï.dds";
            maxlevel = 3;
            maxlearn = 2;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 3;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 7;
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
            talent[0] = 2538;
            talent[1] = 2539;
            talent[2] = 2947;
            talent[3] = 4112;
            talent[4] = 4530;
            talent_size = 5;
            action[0] = "»³¹â_÷Î÷È÷Í÷Ë";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "»³¹â_÷Î÷È÷Í÷Ë";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "»³¹â_Æï³Ë_÷Î÷È÷Í÷Ë";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_÷Î÷È÷Í÷Ë";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 10;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2451Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2450 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[2] = { 140, 141 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               5 + 5 * skill->GetLevel (),
                               244 + 10 * skill->GetLevel (),
                               skill->GetT2 () > 0 || skill->GetT3 () > 0 || skill->GetT4 () > 0 ? table.Find (24513) : table.Find (24514));

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        int GetCoverage (Skill * skill) const
        {
            return (int) (20 +
                          (skill->GetT2 () >=
                           skill->GetT3 ()? (skill->GetT2 () >= skill->GetT4 ()? skill->GetT2 () : skill->GetT4 ()) : (skill->GetT3 () >=
                                                                                                                       skill->GetT4 ()? skill->
                                                                                                                       GetT3 () : skill->GetT4 ())));
        }
#endif
    };
}
#endif
