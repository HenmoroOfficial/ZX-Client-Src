#ifndef __CPPGEN_GNET_SKILL600
#define __CPPGEN_GNET_SKILL600
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill600:public Skill
    {
      public:
        enum
        { SKILL_ID = 600 };
          Skill600 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill600Stub:public SkillStub
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
                return 100;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1);
                skill->SetPlus (0.15 * skill->GetLevel () * (skill->GetPlayer ()->GetHp () + skill->GetPlayer ()->GetMp ()));
                skill->GetPlayer ()->SetMp (0);
                skill->GetPlayer ()->SetHp (1);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill600Stub ():SkillStub (600)
        {
            occupation = 7;
            name = L"¾øÊ¥ÆúÖÇ";
            nativename = "¾øÊ¥ÆúÖÇ";
            icon = "¾øÊ¥ÆúÖÇ.dds";
            maxlevel = 5;
            maxlearn = 3;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            talent_type = 1;
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
            action[0] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[1] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[2] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[3] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[4] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[5] = "ÇàÔÆÃÅ_¾øÊ¥ÆúÖÇ";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_¾øÊ¥ÆúÖÇ";
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
            rangetype = 3;
            doenchant = false;
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
        virtual ~ Skill600Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 597 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (21 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
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
            static int array[3] = { 120, 125, 130 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (20 + skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 22, 20 + skill->GetLevel (), 15 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        int GetCoverage (Skill * skill) const
        {
            return (int) (22);
        }
#endif
    };
}
#endif
