#ifndef __CPPGEN_GNET_SKILL291
#define __CPPGEN_GNET_SKILL291
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill291:public Skill
    {
      public:
        enum
        { SKILL_ID = 291 };
          Skill291 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill291Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill291Stub ():SkillStub (291)
        {
            occupation = 4;
            name = L"ÑÌÓê";
            nativename = "ÑÌÓê";
            icon = "ÑÌÓê.dds";
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
            talent[0] = 757;
            talent[1] = 763;
            talent[2] = 764;
            talent_size = 3;
            action[0] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[1] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[2] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[3] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[4] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[5] = "ºÏ»¶ÅÉ_ÑÌÓê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_ÑÌÓê";
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
        virtual ~ Skill291Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 297 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.56 * skill->GetLevel () * skill->GetLevel () + 1.5 * skill->GetLevel () + 41);
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
            static int array[8] = { 25, 29, 33, 37, 41, 45, 50, 55 };
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
                               INT (0.56 * skill->GetLevel () * skill->GetLevel () + 1.5 * skill->GetLevel () + 41),
                               14 + 2 * skill->GetLevel (),
                               25 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel (),
                               10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((14000 + 2000 * skill->GetLevel ()) * (1 + 0.1 * skill->GetT2 ()) + 100);
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((25 + 50 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.2 * skill->GetT0 ()) * (INT (zrand (100) / (100 - 6 * skill->GetT0 ())) + 1));
            skill->GetVictim ()->SetYanyu (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetLevel () * 10));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetCleardebuff (1);
            return true;
        }
#endif
    };
}
#endif
