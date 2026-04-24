#ifndef __CPPGEN_GNET_SKILL4584
#define __CPPGEN_GNET_SKILL4584
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4584:public Skill
    {
      public:
        enum
        { SKILL_ID = 4584 };
          Skill4584 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4584Stub:public SkillStub
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetPlus (600 + 100 * skill->GetLevel () +
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetPlus (600 + 100 * skill->GetLevel () +
                                skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                            skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                            GetLevel ()));
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetPlus (600 + 100 * skill->GetLevel () +
                                skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                            skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                            GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State5:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes5 ());
                skill->SetPlus (600 + 100 * skill->GetLevel () +
                                skill->GetPlayer ()->GetMaxmp () * 0.002 * (skill->GetPlayer ()->GetSkilllevel (4011) <
                                                                            skill->GetLevel ()? skill->GetPlayer ()->GetSkilllevel (4011) : skill->
                                                                            GetLevel ()));
                skill->GetPlayer ()->SetPerform (0);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
      Skill4584Stub ():SkillStub (4584)
        {
            occupation = 150;
            name = L"ËéÄù¡¤ÆÁÒ¹II";
            nativename = "ËéÄù¡¤ÆÁÒ¹II";
            icon = "ËéÄù¡¤ÆÁÒ¹II.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 11;
            clearmask = 132;
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
            talent[0] = 4152;
            talent_size = 1;
            action[0] = "ÐùÔ¯_·ç±©";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ÐùÔ¯_·ç±©";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÐùÔ¯_Æï³Ë_·ç±©";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ÐùÔ¯_Æï³Ë_·ç±©";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
            doenchant = true;
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
            statestub.push_back (new State5 ());
#endif
        }
        virtual ~ Skill4584Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4027 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000 - 1000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (12);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 20, 600 + 100 * skill->GetLevel (), 5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () + 5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6100);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetSleep (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
