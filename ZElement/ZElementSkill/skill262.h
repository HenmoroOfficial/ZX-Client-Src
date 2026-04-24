#ifndef __CPPGEN_GNET_SKILL262
#define __CPPGEN_GNET_SKILL262
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill262:public Skill
    {
      public:
        enum
        { SKILL_ID = 262 };
          Skill262 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill262Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2400;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill262Stub ():SkillStub (262)
        {
            occupation = 3;
            name = L"ÌìÄ§¸½Ìå";
            nativename = "ÌìÄ§¸½Ìå";
            icon = "ÌìÄ§¸½Ìå.dds";
            maxlevel = 12;
            maxlearn = 5;
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
            skill_class = 0;
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
            talent[0] = 646;
            talent[1] = 666;
            talent_size = 2;
            action[0] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[1] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[2] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[3] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[4] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[5] = "¹íÍõ×Ú_ÌìÄ§¸½Ìå";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÌìÄ§¸½Ìå";
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
        virtual ~ Skill262Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 249 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (36 * skill->GetLevel () + 59 - 0.95 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 6000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 95, 99, 103, 105, 107 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (36 * skill->GetLevel () + 59 - 0.95 * skill->GetLevel () * skill->GetLevel ()),
                               100 * skill->GetLevel (),
                               7 + skill->GetLevel () + INT (skill->GetLevel () / 4) + INT (skill->GetLevel () / 5) * 2, 15);

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
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (100 * skill->GetLevel () * (1 + 0.5 * skill->GetT0 ()));
            skill->GetVictim ()->SetAddhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1800000 + 900000 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.01 * (7 + skill->GetLevel () + INT (skill->GetLevel () / 3.98) + INT (skill->GetLevel () / 4.98) * 2) +
                                           0.01 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncattack (1);
            return true;
        }
#endif
    };
}
#endif
