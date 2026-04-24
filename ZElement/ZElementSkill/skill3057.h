#ifndef __CPPGEN_GNET_SKILL3057
#define __CPPGEN_GNET_SKILL3057
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3057:public Skill
    {
      public:
        enum
        { SKILL_ID = 3057 };
          Skill3057 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3057Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1000;
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
                return 1000;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3057Stub ():SkillStub (3057)
        {
            occupation = 168;
            name = L"ÑªÖä";
            nativename = "ÑªÖä";
            icon = "Ð°¶ñÓ¡¼Ç.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
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
            action[0] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[1] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[2] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[3] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[4] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[5] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[6] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[7] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[8] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[9] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[10] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[11] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[12] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[13] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[14] = "Ä§_ÉñÊ¥×ÆÉÕ";
            action[15] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[16] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[17] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[18] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[19] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[20] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[21] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[22] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[23] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[24] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[25] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[26] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[27] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[28] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
            action[29] = "Æï³Ë_Ä§_ÉñÊ¥×ÆÉÕ";
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
        virtual ~ Skill3057Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (30);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 2000 * skill->GetLevel ();
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
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (18);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               180 + 30 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               4 + skill->GetLevel (),
                               60 - 2 * skill->GetLevel (),
                               1 * skill->GetLevel (), 2 + 0.2 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 30 + 3 * skill->GetLevel ());

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
                                                 (skill->GetPlayer ()->GetType () ==
                                                  0 ? 120 : (skill->GetPlayer ()->GetSkilldodge () > 2000 ? 0 : 120)));
            skill->GetVictim ()->SetTime (4050 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () * 0.1 * (0.2 + skill->GetLevel () * 0.02) * (4 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * 0.1 * (0.2 + skill->GetLevel () * 0.02) * (4 + skill->GetLevel ()) *
                                           (0.1 + 0.02 * skill->GetLevel ()));
            skill->GetVictim ()->SetIgnite (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (4050 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDeccritrate (1);
            skill->GetVictim ()->SetProbability (1.0 * (30 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetAmount (1);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
#endif
    };
}
#endif
