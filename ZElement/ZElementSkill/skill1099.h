#ifndef __CPPGEN_GNET_SKILL1099
#define __CPPGEN_GNET_SKILL1099
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1099:public Skill
    {
      public:
        enum
        { SKILL_ID = 1099 };
          Skill1099 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1099Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus (0.01 * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()) + 4 * skill->GetT0 () * skill->GetLevel () +
                                INT (1.42 - 0.05 * skill->GetLevel ()) * (72 + 13 * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                               0.55) * (0.99 * skill->GetLevel () *
                                                                                                                        skill->GetLevel () +
                                                                                                                        13 * skill->GetLevel () +
                                                                                                                        2.2));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1099Stub ():SkillStub (1099)
        {
            occupation = 25;
            name = L"ÓÄ»êÀû×¦";
            nativename = "ÓÄ»êÀû×¦";
            icon = "ÓÄ»êÀû×¦.dds";
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
            talent[0] = 1159;
            talent[1] = 1163;
            talent[2] = 1167;
            talent[3] = 1172;
            talent_size = 4;
            action[0] = "ÓÄ»êÀû×¦";
            action[1] = "ÓÄ»êÀû×¦";
            action[2] = "ÓÄ»êÀû×¦";
            action[3] = "ÓÄ»êÀû×¦";
            action[4] = "ÓÄ»êÀû×¦";
            action[5] = "ÓÄ»êÀû×¦";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÓÄ»êÀû×¦";
            action[16] = "ÓÄ»êÀû×¦";
            action[17] = "ÓÄ»êÀû×¦";
            action[18] = "ÓÄ»êÀû×¦";
            action[19] = "ÓÄ»êÀû×¦";
            action[20] = "ÓÄ»êÀû×¦";
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
        virtual ~ Skill1099Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1097 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (10 + 2 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 25, 27, 29, 31, 33, 35, 37, 39, 42, 46 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (10 + 2 * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) * (72 + 13 * skill->GetLevel ()) +
                                    INT (0.05 * skill->GetLevel () + 0.55) * (0.99 * skill->GetLevel () * skill->GetLevel () +
                                                                              13 * skill->GetLevel () + 2.2)), INT (45 + 5.5 * skill->GetLevel ()),
                               7 + 3 * skill->GetLevel (), 1 + 0.5 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (45 + 5.5 * skill->GetLevel () + 4 * skill->GetT2 () * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (1500 + 500 * skill->GetLevel () + 100 + 3000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.07 + 0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetWeak (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (15 * skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (0.5 * (40 + 5 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()));
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
