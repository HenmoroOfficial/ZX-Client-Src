#ifndef __CPPGEN_GNET_SKILL1129
#define __CPPGEN_GNET_SKILL1129
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1129:public Skill
    {
      public:
        enum
        { SKILL_ID = 1129 };
          Skill1129 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1129Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 900;
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.04 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 25 * skill->GetT0 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (6.9 * skill->GetLevel () * skill->GetLevel () - 3.6 * skill->GetLevel () +
                                                                          1260) + INT (0.05 * skill->GetLevel () + 0.55) * (428 * skill->GetLevel () -
                                                                                                                            13 * skill->GetLevel () *
                                                                                                                            skill->GetLevel () -
                                                                                                                            860));
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.04 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 25 * skill->GetT0 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (6.9 * skill->GetLevel () * skill->GetLevel () - 3.6 * skill->GetLevel () +
                                                                          1260) + INT (0.05 * skill->GetLevel () + 0.55) * (428 * skill->GetLevel () -
                                                                                                                            13 * skill->GetLevel () *
                                                                                                                            skill->GetLevel () -
                                                                                                                            860));
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
                return 700;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (0.04 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 25 * skill->GetT0 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (6.9 * skill->GetLevel () * skill->GetLevel () - 3.6 * skill->GetLevel () +
                                                                          1260) + INT (0.05 * skill->GetLevel () + 0.55) * (428 * skill->GetLevel () -
                                                                                                                            13 * skill->GetLevel () *
                                                                                                                            skill->GetLevel () -
                                                                                                                            860));
                skill->GetPlayer ()->SetPerform (0);
            }
        };
#endif
      Skill1129Stub ():SkillStub (1129)
        {
            occupation = 28;
            name = L"É­ÂŞÈıĞ¦";
            nativename = "É­ÂŞÈıĞ¦";
            icon = "É­ÂŞÈıĞ¦.dds";
            maxlevel = 20;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 2;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 4;
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
            talent[0] = 1166;
            talent[1] = 1172;
            talent_size = 2;
            action[0] = "É­ÂŞÈıĞ¦";
            action[1] = "É­ÂŞÈıĞ¦";
            action[2] = "É­ÂŞÈıĞ¦";
            action[3] = "É­ÂŞÈıĞ¦";
            action[4] = "É­ÂŞÈıĞ¦";
            action[5] = "É­ÂŞÈıĞ¦";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "É­ÂŞÈıĞ¦";
            action[16] = "É­ÂŞÈıĞ¦";
            action[17] = "É­ÂŞÈıĞ¦";
            action[18] = "É­ÂŞÈıĞ¦";
            action[19] = "É­ÂŞÈıĞ¦";
            action[20] = "É­ÂŞÈıĞ¦";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
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
            statestub.push_back (new State3 ());
            statestub.push_back (new State4 ());
#endif
        }
        virtual ~ Skill1129Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1127 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (735 + 65 * skill->GetLevel () + 3.3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 125, 125, 125, 125, 125 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (735 + 65 * skill->GetLevel () + 3.3 * skill->GetLevel () * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (6.9 * skill->GetLevel () * skill->GetLevel () - 3.6 * skill->GetLevel () + 1260) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (428 * skill->GetLevel () -
                                                                              13 * skill->GetLevel () * skill->GetLevel () - 860)), 15, 16,
                               50 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0.5 * skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
            skill->GetVictim ()->SetCycsubattack (1);
            return true;
        }
#endif
    };
}
#endif
