#ifndef __CPPGEN_GNET_SKILL4009
#define __CPPGEN_GNET_SKILL4009
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4009:public Skill
    {
      public:
        enum
        { SKILL_ID = 4009 };
          Skill4009 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4009Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 150;
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
                return 950;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus (80 + 20 * skill->GetLevel () +
                                skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                            skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                            GetLevel ()));
                skill->GetPlayer ()->SetPerform (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State3:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 950;
            }
            void Calculate (Skill * skill) const
            {
                skill->
                    SetPlus ((80 + 20 * skill->GetLevel () +
                              skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                          skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                          GetLevel ())) * (1 + 0.05 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State4:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 950;
            }
            void Calculate (Skill * skill) const
            {
                skill->
                    SetPlus ((80 + 20 * skill->GetLevel () +
                              skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                          skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                          GetLevel ())) * (1 + 0.10 * skill->GetT0 ()));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill4009Stub ():SkillStub (4009)
        {
            occupation = 57;
            name = L"¾»Äù¡¤ÑÇ¹â";
            nativename = "¾»Äù¡¤ÑÇ¹â";
            icon = "¾»Äù¡¤ÑÇ¹â.dds";
            maxlevel = 10;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 11;
            guide = true;
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
            talent[0] = 4150;
            talent_size = 1;
            action[0] = "ÐùÔ¯_ÉäÏß";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_ÉäÏß";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_ÉäÏß";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_ÉäÏß";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 9;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill4009Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 32 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (50);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 200;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 45, 47, 49, 51, 53, 55, 57, 59, 61 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 80 + 20 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
#endif
    };
}
#endif
