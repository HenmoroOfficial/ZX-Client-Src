#ifndef __CPPGEN_GNET_SKILL236
#define __CPPGEN_GNET_SKILL236
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill236:public Skill
    {
      public:
        enum
        { SKILL_ID = 236 };
          Skill236 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill236Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2600;
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
      Skill236Stub ():SkillStub (236)
        {
            occupation = 9;
            name = L"À×öªÕğÅ­";
            nativename = "À×öªÕğÅ­";
            icon = "À×öªÕğÅ­.dds";
            maxlevel = 12;
            maxlearn = 8;
            type = 3;
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
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 593;
            talent_size = 1;
            action[0] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[1] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[2] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[3] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[4] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[5] = "ÇàÔÆÃÅ_À×öªÕğÅ­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_À×öªÕğÅ­";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 3;
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
        virtual ~ Skill236Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 238 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (21);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.9 * skill->GetLevel () * skill->GetLevel () + 1.2 * skill->GetLevel () + 540);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 80, 82, 84, 86, 88, 90, 92, 94 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (11);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.9 * skill->GetLevel () * skill->GetLevel () + 1.2 * skill->GetLevel () + 540),
                               35,
                               INT (91 + 10 * skill->GetLevel () - 0.18 * skill->GetLevel () * skill->GetLevel ()), 0.7 + 0.3 * skill->GetLevel ());

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
                                                 (91 + 10 * skill->GetLevel () - 0.18 * skill->GetLevel () * skill->GetLevel () +
                                                  3 * skill->GetLevel () * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (700 + 330 * skill->GetLevel () + 100 + 300 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (35);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
