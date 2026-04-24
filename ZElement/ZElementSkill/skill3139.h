#ifndef __CPPGEN_GNET_SKILL3139
#define __CPPGEN_GNET_SKILL3139
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3139:public Skill
    {
      public:
        enum
        { SKILL_ID = 3139 };
          Skill3139 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3139Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 667;
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
                return 2000;
            }
            void Calculate (Skill * skill) const
            {
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
                return 333;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetCrithurt ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.03 * skill->GetLevel ()) : 0));
                skill->SetCrit ((skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.01 * skill->GetLevel ()) : 0));
                skill->SetRatio (1.02 + 0.05 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 0.03 * (skill->GetLevel () - 15) +
                                 (skill->GetCharging () > 250 ? skill->GetCharging () * 0.0005 * (0.5 + 0.1 * skill->GetLevel ()) : 0));
                skill->SetPlus (834);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3139Stub ():SkillStub (3139)
        {
            occupation = 169;
            name = L"ÂÖ»ØÉÁ<ìø>";
            nativename = "ÂÖ»ØÉÁ<ìø>";
            icon = "·ğ¡¤ÂÖ»ØÉÁ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 3;
            targettype = 0;
            autoattack = 1;
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
            skill_class = 6;
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
            talent[0] = 2153;
            talent[1] = 2158;
            talent_size = 2;
            action[0] = "ºüÑı_ÂÖ»ØÉÁ";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "ºüÑı_ÂÖ»ØÉÁ";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºüÑı_Æï³Ë_ÂÖ»ØÉÁ";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "ºüÑı_Æï³Ë_ÂÖ»ØÉÁ";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
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
            weapon_limit = 8;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
            statestub.push_back (new State3 ());
#endif
        }
        virtual ~ Skill3139Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1921 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
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
            return (float) (40 + 0.5 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 5);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (866 + 14 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 40000 - 1000 * skill->GetLevel () - 5000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (30 + 0.5 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20 + 0.5 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 5);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               866 + 14 * skill->GetLevel (),
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               40 - skill->GetLevel (),
                               20 + 0.8 * skill->GetLevel (),
                               INT (2 + INT (5 * skill->GetLevel () + norm (skill->GetLevel () / 14.9) * 3 * (skill->GetLevel () - 15))),
                               834, 20 + 4 * skill->GetLevel (), 50 + 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 4 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetReborncount () + 1) * skill->GetPlayer ()->GetLevel () * 50);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (6000 + 2000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount ((skill->GetPlayer ()->GetReborncount () + 1) * skill->GetPlayer ()->GetLevel () * 50);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxmp () - skill->GetPlayer ()->GetMp ());
            skill->GetVictim ()->SetSubmp (1);
            return true;
        }
#endif
    };
}
#endif
