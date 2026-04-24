#ifndef __CPPGEN_GNET_SKILL1101
#define __CPPGEN_GNET_SKILL1101
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1101:public Skill
    {
      public:
        enum
        { SKILL_ID = 1101 };
          Skill1101 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1101Stub:public SkillStub
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
                skill->SetRatio (1 + 0.05 * skill->GetT1 ());
                skill->SetPlus (INT (1.42 - 0.05 * skill->GetLevel ()) * (150 + 7 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) +
                                INT (0.05 * skill->GetLevel () + 0.55) * (2.3 * skill->GetLevel () * skill->GetLevel () - 12 * skill->GetLevel () +
                                                                          210));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1101Stub ():SkillStub (1101)
        {
            occupation = 25;
            name = L"ÊÉ»êÑªÑÀ";
            nativename = "ÊÉ»êÑªÑÀ";
            icon = "ÊÉ»êÑªÑÀ.dds";
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
            talent[0] = 1160;
            talent[1] = 1172;
            talent_size = 2;
            action[0] = "ÊÉ»êÑªÑÀ";
            action[1] = "ÊÉ»êÑªÑÀ";
            action[2] = "ÊÉ»êÑªÑÀ";
            action[3] = "ÊÉ»êÑªÑÀ";
            action[4] = "ÊÉ»êÑªÑÀ";
            action[5] = "ÊÉ»êÑªÑÀ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÊÉ»êÑªÑÀ";
            action[16] = "ÊÉ»êÑªÑÀ";
            action[17] = "ÊÉ»êÑªÑÀ";
            action[18] = "ÊÉ»êÑªÑÀ";
            action[19] = "ÊÉ»êÑªÑÀ";
            action[20] = "ÊÉ»êÑªÑÀ";
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
#endif
        }
        virtual ~ Skill1101Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1099 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (15 + 3 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 5000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 35, 37, 39, 41, 43, 45, 47, 49, 52, 56 };
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
                               INT (15 + 3 * skill->GetLevel ()),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (150 + 7 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                    0.55) * (2.3 *
                                                                                                                             skill->GetLevel () *
                                                                                                                             skill->GetLevel () -
                                                                                                                             12 * skill->GetLevel () +
                                                                                                                             210)),
                               INT (15 + 5 * (skill->GetLevel () - 1)),
                               INT (INT (1.42 - 0.05 * skill->GetLevel ()) *
                                    (150 + 7 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) + INT (0.05 * skill->GetLevel () +
                                                                                                                    0.55) * (2.3 *
                                                                                                                             skill->GetLevel () *
                                                                                                                             skill->GetLevel () -
                                                                                                                             12 * skill->GetLevel () +
                                                                                                                             210)), 5);

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (norm
                                                  ((0.15 + 0.05 * (skill->GetLevel () - 1) +
                                                    0.05 * skill->GetT0 ()) * skill->GetPlayer ()->GetMaxhp () / (skill->GetPlayer ()->GetHp () +
                                                                                                                  1)) * 100));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->
                SetAmount ((INT (1.42 - 0.05 * skill->GetLevel ()) * (150 + 7 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) +
                            INT (0.05 * skill->GetLevel () + 0.55) * (2.3 * skill->GetLevel () * skill->GetLevel () - 12 * skill->GetLevel () +
                                                                      210)) * (1 + 0.2 * skill->GetT0 ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
#endif
    };
}
#endif
