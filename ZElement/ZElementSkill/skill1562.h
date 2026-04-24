#ifndef __CPPGEN_GNET_SKILL1562
#define __CPPGEN_GNET_SKILL1562
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1562:public Skill
    {
      public:
        enum
        { SKILL_ID = 1562 };
          Skill1562 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1562Stub:public SkillStub
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
                skill->SetRatio (1 + 0.02 * skill->GetLevel ());
                skill->
                    SetPlus ((skill->GetPlayer ()->GetRes1 () + skill->GetPlayer ()->GetRes2 () + skill->GetPlayer ()->GetRes3 () +
                              skill->GetPlayer ()->GetRes4 () + skill->GetPlayer ()->GetRes5 ()) * 0.5 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1562Stub ():SkillStub (1562)
        {
            occupation = 149;
            name = L"ÔÆÓêII";
            nativename = "ÔÆÓêII";
            icon = "ÔÆÓêII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[1] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[2] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[3] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[4] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[5] = "ºÏ»¶ÅÉ_ÏÐÓê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÏÐÓê";
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
        virtual ~ Skill1562Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 532 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950 + 35 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 16000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
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
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               950 + 35 * skill->GetLevel (),
                               2 * skill->GetLevel (), 0.5 * skill->GetLevel (), 60 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (60 * skill->GetLevel () *
                                           (3 - 2 * (skill->GetPlayer ()->GetMp () + 1) / (skill->GetPlayer ()->GetMaxmp () + 1)));
            skill->GetVictim ()->SetSubdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (8000 + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () *
                                           (3 - 2 * (skill->GetPlayer ()->GetMp () + 1) / (skill->GetPlayer ()->GetMaxmp () + 1)));
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecskillaccu (1);
            return true;
        }
#endif
    };
}
#endif
