#ifndef __CPPGEN_GNET_SKILL307
#define __CPPGEN_GNET_SKILL307
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill307:public Skill
    {
      public:
        enum
        { SKILL_ID = 307 };
          Skill307 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill307Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetHp () * 1000 / skill->GetPlayer ()->GetMaxhp ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill307Stub ():SkillStub (307)
        {
            occupation = 5;
            name = L"Æí¸£";
            nativename = "Æí¸£";
            icon = "Æí¸£.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            skill_class = 1;
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
            talent[0] = 759;
            talent[1] = 763;
            talent[2] = 764;
            talent[3] = 740;
            talent_size = 4;
            action[0] = "ºÏ»¶ÅÉ_Æí¸£";
            action[1] = "ºÏ»¶ÅÉ_Æí¸£";
            action[2] = "ºÏ»¶ÅÉ_Æí¸£";
            action[3] = "ºÏ»¶ÅÉ_Æí¸£";
            action[4] = "ºÏ»¶ÅÉ_Æí¸£";
            action[5] = "ºÏ»¶ÅÉ_Æí¸£";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_Æí¸£";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
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
        virtual ~ Skill307Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 294 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.097 * skill->GetLevel () * skill->GetLevel () + 21 * skill->GetLevel () + 70);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 14400 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 70, 73, 76, 79, 82, 85, 87, 89 };
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
                               INT (0.097 * skill->GetLevel () * skill->GetLevel () + 21 * skill->GetLevel () + 70),
                               14 + 2 * skill->GetLevel (), 15 + 15 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((14000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->
                SetRatio ((0.15 + 0.15 * skill->GetLevel () + 0.01 * skill->GetLevel () * skill->GetLevel () +
                           0.06 * skill->GetT0 () * skill->GetLevel ()) * (INT (zrand (100) / (100 - 6 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetInchp2 (1);
            skill->GetVictim ()->SetTime ((1800000 + 900000 * skill->GetT3 ()) * norm (skill->GetPlayer ()->GetSkilllevel (305)) *
                                          norm (skill->GetPlayer ()->GetSkilllevel (740) / 1.5));
            skill->GetVictim ()->SetBuffid (4);
            skill->GetVictim ()->
                SetValue ((0.2 * skill->GetPlayer ()->GetSkilllevel (305) * skill->GetPlayer ()->GetLevel () +
                           0.05 * skill->GetPlayer ()->GetSkilllevel (305) * skill->GetPlayer ()->GetMaxmp ()) * (1 + 0.35 * skill->GetT3 ()));
            skill->GetVictim ()->SetAddhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 () > 950 ? 120 * norm (skill->GetT0 () / 1.5) : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetHeal (1);
            return true;
        }
#endif
    };
}
#endif
