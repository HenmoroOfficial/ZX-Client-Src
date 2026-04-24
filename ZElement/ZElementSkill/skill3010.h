#ifndef __CPPGEN_GNET_SKILL3010
#define __CPPGEN_GNET_SKILL3010
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3010:public Skill
    {
      public:
        enum
        { SKILL_ID = 3010 };
          Skill3010 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3010Stub:public SkillStub
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
      Skill3010Stub ():SkillStub (3010)
        {
            occupation = 167;
            name = L"±±Ú¤Ó¡";
            nativename = "±±Ú¤Ó¡";
            icon = "±ù·âÊõ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
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
            preskillnum = 0;
            action[0] = "ÏÉ_±ù±¬";
            action[1] = "ÏÉ_±ù±¬";
            action[2] = "ÏÉ_±ù±¬";
            action[3] = "ÏÉ_±ù±¬";
            action[4] = "ÏÉ_±ù±¬";
            action[5] = "ÏÉ_±ù±¬";
            action[6] = "ÏÉ_±ù±¬";
            action[7] = "ÏÉ_±ù±¬";
            action[8] = "ÏÉ_±ù±¬";
            action[9] = "ÏÉ_±ù±¬";
            action[10] = "ÏÉ_±ù±¬";
            action[11] = "ÏÉ_±ù±¬";
            action[12] = "ÏÉ_±ù±¬";
            action[13] = "ÏÉ_±ù±¬";
            action[14] = "ÏÉ_±ù±¬";
            action[15] = "Æï³Ë_ÏÉ_±ù±¬";
            action[16] = "Æï³Ë_ÏÉ_±ù±¬";
            action[17] = "Æï³Ë_ÏÉ_±ù±¬";
            action[18] = "Æï³Ë_ÏÉ_±ù±¬";
            action[19] = "Æï³Ë_ÏÉ_±ù±¬";
            action[20] = "Æï³Ë_ÏÉ_±ù±¬";
            action[21] = "Æï³Ë_ÏÉ_±ù±¬";
            action[22] = "Æï³Ë_ÏÉ_±ù±¬";
            action[23] = "Æï³Ë_ÏÉ_±ù±¬";
            action[24] = "Æï³Ë_ÏÉ_±ù±¬";
            action[25] = "Æï³Ë_ÏÉ_±ù±¬";
            action[26] = "Æï³Ë_ÏÉ_±ù±¬";
            action[27] = "Æï³Ë_ÏÉ_±ù±¬";
            action[28] = "Æï³Ë_ÏÉ_±ù±¬";
            action[29] = "Æï³Ë_ÏÉ_±ù±¬";
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
        virtual ~ Skill3010Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 4000 * skill->GetLevel ();
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
            return (float) (12);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               180 - 4 * skill->GetLevel (),
                               2 + 0.2 * skill->GetLevel (),
                               10 + 2 * skill->GetLevel (), skill->GetLevel (), 2 * skill->GetLevel (), 20 + 4 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetIsfrozen ()? 120 : 0));
            skill->GetVictim ()->SetTime (2050 + 200 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 2000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (-20 - 4 * skill->GetLevel ());
            skill->GetVictim ()->SetDivinityburst (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2050 + 200 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 2000);
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->SetAmount (1000);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp () * 0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (2050 + 200 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 2000);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdrugeffect (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetIsfrozen ()? 35 + 2 * skill->GetLevel () : 10 + 2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (2050 + 200 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 2000);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetFrozen (1);
            return true;
        }
#endif
    };
}
#endif
