#ifndef __CPPGEN_GNET_SKILL4004
#define __CPPGEN_GNET_SKILL4004
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4004:public Skill
    {
      public:
        enum
        { SKILL_ID = 4004 };
          Skill4004 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4004Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 775;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetPlus ((skill->GetPlayer ()->GetLevel () * 6 + skill->GetPlayer ()->GetMaxmp () * skill->GetLevel () / 100) *
                                (1 +
                                 0.1 *
                                 ((skill->GetPlayer ()->GetDarklightform () == 4 || skill->GetPlayer ()->GetDarklightform () == 5
                                   || skill->GetPlayer ()->GetDarklightform () == 6) ? skill->GetT2 () : 0)));
                skill->GetPlayer ()->SetDarklight (skill->GetPlayer ()->GetDarklight () +
                                                   (skill->GetLevel () + skill->GetT0 ()) * (skill->GetPlayer ()->GetDarklightform () == 5 ? -1 : 1));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4004Stub ():SkillStub (4004)
        {
            occupation = 56;
            name = L"¾»Äù¡¤î¸¡¤¾²Óê";
            nativename = "¾»Äù¡¤î¸¡¤¾²Óê";
            icon = "¾»Äù¡¤î¸¡¤¾²Óê.dds";
            maxlevel = 10;
            maxlearn = 9;
            type = 1;
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
            skill_class = 11;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 2;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 4132;
            talent[1] = 4133;
            talent[2] = 4134;
            talent[3] = 4138;
            talent[4] = 4143;
            talent[5] = 4149;
            talent_size = 6;
            action[0] = "ĞùÔ¯_¹â·Éµ¯";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "ĞùÔ¯_¹â·Éµ¯";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ĞùÔ¯_Æï³Ë_¹â·Éµ¯";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "ĞùÔ¯_Æï³Ë_¹â·Éµ¯";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = false;
            dobless = true;
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
#endif
        }
        virtual ~ Skill4004Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4002 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20 + skill->GetT5 () * 2);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetLevel () * 2);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1275;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 2000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 15, 17, 19, 21, 23, 25, 27, 29, 31 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17 + skill->GetT5 () * 2);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + skill->GetT5 () * 2);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetPlayer ()->GetLevel () * 2, skill->GetLevel (), skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT3 () > 0 ? 120 : 0));
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetLevel () * 5 * 0.2 * skill->GetT3 ());
            skill->GetVictim ()->SetHeal (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT4 () * 5));
            skill->GetVictim ()->SetRatio (skill->GetPlayer ()->GetDarklightform () > 5 ? 3 : 2);
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetAddspirit (1);
            return true;
        }
#endif
    };
}
#endif
